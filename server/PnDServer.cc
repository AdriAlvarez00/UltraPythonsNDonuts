#include <thread>
#include "SnakeServer.h"

int main(int argc, char** argv){
    SnakeServer server(argv[1],argv[2]);
    //SnakeServer server("127.0.0.1", "55555");
    std::cout<< "Escuchando en "<< argv[1] << " : "<< argv[2]<< "\n";
    std::thread game_thread([&server](){server.run_logic();});
    
    server.handle_messages();
}