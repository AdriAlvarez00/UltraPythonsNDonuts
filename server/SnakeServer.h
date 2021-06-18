#include "Socket.h"

class SnakeServer{
    private:
    Socket socket;
    std::vector<std::unique_ptr<Socket>> clients;
    public:
    void requested_connection(){}
    void received_input(){}
    void broadcast_state(){}
    void do_messages();
    SnakeServer(const char* host,const char* port):socket(host,port){
        socket.bind();
    }

};