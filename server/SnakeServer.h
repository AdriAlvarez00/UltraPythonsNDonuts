#pragma once
#include "SnakeGames.h"
#include "Socket.h"
#include "memory.h"
#include "SnakeMessages.h"

class SnakeServer{
    private:
        Socket socket;
        //Almacenaremos el input de los jugadores recibidos, debera ir protegido con un mutex
        std::vector<Vector2> receivedInputs;
        std::vector<std::pair<uint32_t,std::unique_ptr<Socket>>> clients;
        uint32_t connectedPlayers;
        GameState gameState;

    public:
    SnakeServer(const char* host,const char* port):socket(host,port),connectedPlayers(0),clients(0){
        socket.bind();
    }
    //Devolvera el id del jugador o -1 si no esta conectado
    uint32_t get_connected_id(Socket* socket);
    void handle_input(Socket* sock,Vector2 input);
    void on_connection_requested(Socket* sock,LoginPetition& petition);
    void handle_messages();
    void run_logic();
};