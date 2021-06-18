#include "snake.pb.h"
#include "SnakeGames.h"
#include "SnakeServer.h"

int main(int argc, char** argv){
    SnakeServer server(argv[1],argv[2]);

    server.do_messages();

    return 1;
}