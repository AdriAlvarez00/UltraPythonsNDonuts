#include <thread>
#include "SnakeServer.h"

int main(int argc, char** argv){
    //Uno de cada packetloss paquetes se perdera
    int packetloss = 0;
    if(argc == 4)
        packetloss = atoi(argv[3]);
    
    SnakeServer server(argv[1],argv[2],packetloss);
    //SnakeServer server("127.0.0.1", "55555");
    std::cout<< "Escuchando en "<< argv[1] << " : "<< argv[2]<< "\n";
    std::thread game_thread([&server](){server.run_logic();});
    
    server.handle_messages();
}