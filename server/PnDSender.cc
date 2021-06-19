#include "Socket.h"
#include "SnakeGames.h"
#include "SnakeMessages.h"

int main(int argc,char** argv){
    Socket serverSock("127.0.0.1","55555");
    Socket clientSock("127.0.0.1","22222");
    int id=0;
    if(argc == 2)
        id = atoi(argv[1]);

    if(id == 0){
        LoginPetition petition("Paco");
        serverSock.send(petition,serverSock,0);
    }
    else if(id == 1){

    }
    else if(id == 48){
        Snake serpi(id);
        clientSock.send(serpi,clientSock,48);
    }

    return 0;
}