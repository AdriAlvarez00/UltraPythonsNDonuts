#include "SnakeServer.h"
#include "SnakeMessages.h"
#include <chrono>
#include <thread>

/*
* 0 login petition
* 1 login response
* 2 input update
* 3 gamestate
*/
uint32_t SnakeServer::get_connected_id(Socket *sock)
{
    auto it = clients.begin();
    while (it != clients.end() && *(*it).second != *sock)
        ++it;
    return (it == clients.end()) ? -1 : it->first;
}

void SnakeServer::on_connection_requested(Socket *sc, LoginPetition &pet)
{
    std::cout << "handling connection";
    std::cout << "CONNECTING: " << pet.nick;
    LoginResponse response(++connectedPlayers, gameState.getGridSize());
    socket.send(response, *sc, 1);
    std::unique_ptr<Socket> uptr = std::make_unique<Socket>(*sc);
    sc = nullptr;
    clients.push_back(std::make_pair(response.uid, std::move(uptr)));

    int halfG = gameState.getGridSize() / 2;
    int quartG = halfG / 2;
    int distAlDonut = 2;

    mtx_state.lock();
    if (response.uid == 1)
    {
        //Snake s(response.uid, {Vector2(7, 7), Vector2(7, 6)}, Vector2(0, 1));
        Snake s (response.uid, Vector2(quartG), Vector2(0,1), 2);
        gameState.addSnake(s);
    }
    else if(response.uid == 2)
    {
        //Snake s(response.uid, {Vector2(halfG + quartG), Vector2(22, 23)}, Vector2(0, -1));
        Snake s (response.uid, Vector2(halfG + quartG), Vector2(0,-1), 2);
        gameState.addSnake(s);
    }
        else if(response.uid == 3)
    {
        //Snake s(response.uid, {Vector2(22, 7), Vector2(23,7)}, Vector2(-1, 0));
        Snake s (response.uid, Vector2(halfG + quartG, quartG), Vector2(-1,0), 2);
        gameState.addSnake(s);
    }
        else if(response.uid == 4)
    {
        //Snake s(response.uid, {Vector2(7, 22), Vector2(6, 22)}, Vector2(1, 0));
        Snake s (response.uid, Vector2(quartG, halfG + quartG), Vector2(1,0), 2);
        gameState.addSnake(s);
    }
    else if(response.uid == 5)
    {        
        Snake s (response.uid, Vector2(halfG - distAlDonut), Vector2(0,-1), 2);
        gameState.addSnake(s);
    }
    else if(response.uid == 6)
    {        
        Snake s (response.uid, Vector2(halfG + distAlDonut, halfG -2), Vector2(-1,0), 2);
        gameState.addSnake(s);
    }
    else if(response.uid == 7)
    {        
        Snake s (response.uid, Vector2(halfG + distAlDonut), Vector2(0,1), 2);
        gameState.addSnake(s);
    }
    else if(response.uid == 8)
    {        
        Snake s (response.uid, Vector2(halfG - distAlDonut, halfG + 2), Vector2(1,0), 2);
        gameState.addSnake(s);
    }

    mtx_state.unlock();
    //Aumentamos el buffer de input
    receivedInputs.push_back(Vector2(0, 0));
}

void SnakeServer::broadcast_game_state()
{
    if(dbg_packet_loss !=0 && gameState.get_tick()%dbg_packet_loss==0)
        return;
    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        socket.send(gameState, *(*it).second, MessageID::GAMESTATE);
    }
}

void SnakeServer::broadcast_game_over()
{
    uint32_t idW = gameState.getWinner();
    if(idW < 1){
        std::cout << "Error, el id del ganador no puede ser menor que 1 [SnakeServer.cpp]\n";
        return;        
    }

    Winner winner(idW);

    for (auto it = clients.begin(); it != clients.end(); ++it)
    {
        socket.send(winner, *(*it).second, MessageID::GAMEOVER);
    }
}

void SnakeServer::handle_messages()
{
    while (true)
    {
        Socket *msgSock = new Socket(socket);
        json recv = socket.recvObj(msgSock);
        int id = recv["ID"];
        std::cout << "handling " << id << std::endl;
        if (id == MessageID::LOGINPETITION)
        {
            if (get_connected_id(msgSock) == -1)
            {
                LoginPetition petition("ayuda");
                petition.from_bin(recv["OBJ"]);
                on_connection_requested(msgSock, petition);
            }
        }
        else if (id == MessageID::LOGINRESPONSE)
        {
            // Nunca deberian llegarnos
        }
        else if (id == MessageID::INPUT)
        {
            Vector2 inputR(0, 0);
            inputR.from_bin(recv["OBJ"]);
            handle_input(msgSock, inputR);
        }
        else if (id == MessageID::REQUESTSTART)
        {
            if (connectedPlayers > 1 && get_connected_id(msgSock)==1)
            {
                StartGame start;
                for (auto it = clients.begin(); it != clients.end(); ++it)
                {
                    socket.send(start, *(*it).second, MessageID::GAMESTART);
                }
                state = ServerState::RUNNNING;
            }
        }
    }
};

void SnakeServer::run_logic()
{
    while (true)
    {        
        if (state == ServerState::RUNNNING)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(333));

            mtx_input.lock();
            for (int i = 0; i < connectedPlayers; i++)
            {
                gameState.moveSnake(i + 1, receivedInputs[i]);
            }
            mtx_input.unlock();
            mtx_state.lock();

            gameState.update();
            gameState.draw();
            
            //No tiene sentido hacer broadcast si no actualizamos el estado;
            std::cout<<"ganador :"<< gameState.getWinner() << "\n";

            if(gameState.getWinner() > 0 ) state = ServerState::OVER ;
            else broadcast_game_state();

            mtx_state.unlock();
        }
        else if (state == ServerState::WAITING)
        {
            //Si no esperamos un poco no podemos ver el estado en el servidor
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            gameState.draw();
            //No tiene sentido hacer broadcast si no actualizamos el estado;
            broadcast_game_state();
        }
        else if(state == ServerState::OVER)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));          
            broadcast_game_state();
            broadcast_game_over();
        }
        std::cout<<"Estado actual: " << state << "\n";
    }
}

void SnakeServer::handle_input(Socket *sock, Vector2 input)
{
    //Comprobamos que el input no haya sido manipulado
    if (input.getX() > 1 || input.getY() > 1 || input.getX() < -1 || input.getY() < -1)
    {
        std::cout << "TRAMPOSO";
        return;
    }
    //Comprobamos que sea una direccion valida
    if (input.getX() == 0 && input.getY() == 0)
    {
        std::cout << "Input vacio\n";
        return;
    }

    //Modificamos el input del jugador correspondiente, se podria usar un MAP pero para
    //tan pocos jugadores no creo que afecte, podria ser hasta peor el coste logaritmico
    //mtx_input.lock();
    //std::cout << "connid: " << get_connected_id(sock) << std::endl;
    receivedInputs[get_connected_id(sock) - 1] = input;
    mtx_input.unlock();
}