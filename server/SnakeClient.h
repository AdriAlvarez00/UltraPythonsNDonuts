#include "Socket.h"

class SnakeClient{
           
    private:

    Socket socket;

    /**
     * Nick del usuario
     */
    std::string nick;
    public:
        SnakeClient(const char* host,const char* port):socket(host,port){}
        bool Connect();
};