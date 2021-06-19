#include "SnakeServer.h"
int main(int argc, char** argv){
    SnakeServer server(argv[1],argv[2]);

    while(true){
        server.handle_messages();
    }
}