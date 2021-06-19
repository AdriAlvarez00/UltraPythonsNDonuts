#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>
#include <stdlib.h>
#include "Socket.h"

#define GRID_SIZE 20
#define SNAKE X
#define WALL #

int main()
{

	Snake serpi(1);
	// serpi.to_bin();

	// std::ofstream file;
	// std::ifstream fileIn;
	// file.open("snake.data");
	// PnD::Snake* msg = serpi.get_message();
	// int siz = msg->ByteSizeLong();
	// msg->SerializeToOstream(&file);
	// file.close();

	// fileIn.open("snake.data");
	// std::string buf2;
	// fileIn >> buf2;
	// serpi.print(std::cout);
	// std::cout << std::endl;
	// Snake serpi2(2);
	// PnD::Snake* aux = new PnD::Snake();
	// aux->ParsePartialFromIstream(&fileIn);
	// serpi2.from_message(aux);
	// serpi2.print(std::cout);

	bool running = true;
	Vector2 fruit(5, 5);
	Socket sock("127.0.0.1", "55555");
	sock.bind();
	int i = 0;

	Socket *msgSock = new Socket(sock);

	while (true)
	{
		std::cout << "Waiting for next msg" << std::endl;
		//sock.recvHeader(msgSock, &header);
		//std::cout << "received: " << header.getID() << std::endl;
		json obj = sock.recvObj(msgSock);
		
		if (obj["ID"]== 48)
		{
			std::cout << "Cargando serpiente";
			serpi.from_bin(obj["OBJ"]);
		}
		//std::cout << "llego una serpi con id: " << serpi.getSnakeID() << std::endl;
		serpi.print(std::cout);
	}

	while (running)
	{
		sock.recvObj(msgSock);

		if (serpi.getHead() == fruit)
			serpi.increaseLength(5);
		serpi.move();
		//draw(serpi, fruit);
		std::cout << i;
		i++;
	}
}
