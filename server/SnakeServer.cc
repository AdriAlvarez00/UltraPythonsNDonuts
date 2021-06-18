#include "SnakeServer.h"
#include "SnakeGames.h"

void SnakeServer::do_messages()
{
    Socket *msgSock = new Socket(socket);
    Header header;
    socket.recvHeader(msgSock,&header);

    std::cout << "in header: " << std::endl;
    if (header.getID() == PnD::MessageID::LOGINPETITION)
    {
        LoginPetition request("none");
        socket.loadObj(request,msgSock);
        std::cout << "LOGIN " << request.getName() << "\n";
        std::unique_ptr<Socket> uptr = std::make_unique<Socket>(*msgSock);
        msgSock = nullptr;
        clients.push_back(std::move(uptr));
    }
    else{
        std::cout << "UNKNOWN MESSAGE\n";
    }
}