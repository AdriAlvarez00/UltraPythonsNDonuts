#include "Socket.h"
#include "SnakeGames.h"


int main(int argc,char** argv){
    Socket sock("127.0.0.1","55555");
    int id=0;
    if(argc == 2)
        id = atoi(argv[1]);
    Snake serpi(id);
    sock.send(serpi,sock,48);
    return 0;
}