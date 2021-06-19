#include <thread>
#include "SnakeServer.h"

int main(int argc, char** argv){
    SnakeServer server(argv[1],argv[2]);

    std::thread game_thread([&server](){server.run_logic();});

    server.handle_messages();
}