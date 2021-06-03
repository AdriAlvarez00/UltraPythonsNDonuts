#include <string.h>
#include <sys/socket.h>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
	addrinfo hints;
	addrinfo *result;
	memset(&hints,0,sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM; //UDP
	hints.ai_family = AF_INET;//IPV4

	int retcode = getaddrinfo(address,port,&hints,&result);

	if(retcode!=0){
		std::cerr << gai_strerror(retcode) << std::endl;
	}

	sd = socket(result->ai_family,result->ai_socktype, 0);
	if(sd == -1)
	{
		std::cerr << "[socket] failed to create errno " << sd;
	}

	sa = *result->ai_addr;
	sa_len = result->ai_addrlen;
}

int Socket::recv(Serializable<google::protobuf::Message> &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable<google::protobuf::Message>& obj, const Socket& sock)
{
    //Serializar el objeto
    obj.to_bin();
    //Enviar el objeto binario a sock usando el socket sd
    int rc = sendto(sd,obj.data().c_str(),obj.size(),0,&sock.sa,sock.sa_len);
    return (rc!=-1)?0:-1;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    sockaddr_in *in1 = (sockaddr_in*) &(s1.sa);
    sockaddr_in *in2 = (sockaddr_in*) &(s2.sa);

    return (in1->sin_family == in2->sin_family && in1->sin_addr.s_addr == in2->sin_addr.s_addr && in1->sin_port == in2->sin_port);
};

bool operator!= (const Socket &s1, const Socket &s2){
	return !(s1==s2);
}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

