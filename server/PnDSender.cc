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
    else if(id == 2){
        LoginPetition petition("Paco");
        serverSock.send(petition,serverSock,0);
        bool running = true;
        while(running){
        char in = getchar();
		char enter = getchar();
        Vector2 v(0,0);
		switch (in)
		{
		case 'a':
			v = Vector2(-1, 0);
			break;
		case 'd':
			v = Vector2(1, 0);
			break;
		case 'w':
			v = Vector2(0,-1);
			break;
		case 's':
			v = Vector2(0, 1);
			break;
		case 'b':
			running = false;
			break;
		default:
			break;
		}
            serverSock.send(v,serverSock,2);
        }
    }
    else if(id == 48){
        Snake serpi;
        clientSock.send(serpi,clientSock,48);
    }

    return 0;
}