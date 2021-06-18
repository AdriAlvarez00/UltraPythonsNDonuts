#include "SnakeClient.h"
#include "SnakeGames.h"

bool SnakeClient::Connect(){
    std::string nombre;
    std::cin >> nombre;
    LoginPetition peticion(nombre.c_str());
    Snake serpi(3);
    socket.send(serpi,socket,PnD::MessageID::GAMEUPDATE);

    return true;
}