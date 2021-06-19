#include "SnakeServer.h"
#include "SnakeMessages.h"

/*
* 0 login petition
* 1 login response
* 2 input update
* 3 gamestate
*/
bool SnakeServer::is_connected(Socket* sock){
    auto it = clients.begin();
    while(it!=clients.end() && *(*it).second != *sock)
        ++it;
    return it!=clients.end();
}

void SnakeServer::on_connection_requested(Socket* sc,LoginPetition& pet ){
    std::cout << "handling connection";
    std::cout << "CONNECTING: " << pet.nick;
    LoginResponse response(++connectedPlayers);
    socket.send(response,*sc,1);
    std::unique_ptr<Socket> uptr = std::make_unique<Socket>(*sc);
    sc=nullptr;
    clients.push_back(std::make_pair(response.uid,std::move(uptr)));
}

void SnakeServer::handle_messages(){
  Socket* msgSock = new Socket(socket);
  json recv = socket.recvObj(msgSock);
  int id = recv["ID"];
  std::cout << "handling " << id << std::endl;
  if(id == 0){
    if(!is_connected(msgSock)){
        LoginPetition petition("ayuda");
        petition.from_bin(recv["OBJ"]);
        on_connection_requested(msgSock,petition);
        }
  }
  else if(id == 2){

  }

};