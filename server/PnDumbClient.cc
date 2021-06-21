//Cliente sin input, solo prueba a dibujar el estado que recibe

#include "Socket.h"
#include "SnakeGames.h"
#include "SnakeMessages.h"

int main(){
    Socket serverSock("127.0.0.1","55555");
    LoginPetition petition("ObservadorNeutral");
    serverSock.send(petition,serverSock,0);
    Socket* msgSock = new Socket(serverSock);
    GameState gs;
    while(true){

        json msg = serverSock.recvObj(msgSock);
        int id = msg["ID"];
        if(id==3){
            gs.from_bin(msg["OBJ"]);
        }
        else{
            std::cout << "received: " << id << std::endl;
        }
        gs.draw();
    }


}