#include "Socket.h"
#include "SnakeGames.h"
#include "SnakeMessages.h"

int main(){
    Socket serverSock("127.0.0.1","55555");
    LoginPetition petition("Low Orbit Tirachinas");
    serverSock.send(petition,serverSock,0);
    while (true)
    {
        Vector2 v(5,5);
        serverSock.send(v,serverSock,2);
    }
    
}