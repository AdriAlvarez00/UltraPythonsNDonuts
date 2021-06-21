#pragma once
#include "SnakeGames.h"
#include "Socket.h"
#include "memory.h"
#include "SnakeMessages.h"
#include <thread>
#include <mutex>

class SnakeServer
{
private:
    Socket socket;
    //Almacenaremos el input de los jugadores recibidos, debera ir protegido con un mutex
    //Esta solucion sencilla nos permitira aplicar siempre el input mas reciente del jugador
    //Y el juego nunca se vera ralentizado por culpa de un mensaje de input, a diferencia de si
    //Usasemos el mutex para el estado de juego
    std::mutex mtx_input;
    std::mutex mtx_state;
    std::vector<Vector2> receivedInputs;

    std::vector<std::pair<uint32_t, std::unique_ptr<Socket>>> clients;
    uint32_t connectedPlayers;
    GameState gameState;

    int dbg_packet_loss;

    enum MessageID
    {
        LOGINPETITION = 0,
        LOGINRESPONSE,
        INPUT,
        GAMESTATE,
        GAMESTART,
        REQUESTSTART,
        QUIT,
        GAMEOVER,
    };

    enum ServerState
    {
        WAITING = 0,
        RUNNNING,
        OVER
    };
    ServerState state;

public:
    SnakeServer(const char *host, const char *port,int packetloss) : socket(host, port),dbg_packet_loss(packetloss), connectedPlayers(0), clients(0)
    {
        socket.bind();
        state = ServerState::WAITING;
    }
    //Devolvera el id del jugador o -1 si no esta conectado
    uint32_t get_connected_id(Socket *socket);
    void handle_input(Socket *sock, Vector2 input);
    void on_connection_requested(Socket *sock, LoginPetition &petition);
    void broadcast_game_state();
    void broadcast_game_over();
    void handle_messages();
    void run_logic();
};