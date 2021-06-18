#include "snake.pb.h"
#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>
#include <stdlib.h>
#include "Socket.h"

#define GRID_SIZE 20
#define SNAKE X
#define WALL #

void draw(const Snake &snake, const Vector2 &fruit)
{
	printf("\033c");
	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			if (x == 0 || x == GRID_SIZE - 1 || y == 0 || y == GRID_SIZE - 1)
			{
				printf("#");
			}
			else if (Vector2(x, y) == fruit)
			{
				printf("*");
			}
			//No es eficiente pero es debug
			else if (snake.collidesWithBody(Vector2(x, y)))
			{
				printf("O");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

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

	bool running = true;
	Vector2 fruit(5, 5);
	Socket sock("127.0.0.1", "55555");
	sock.bind();
	Header header;
	int i = 0;

	Socket *msgSock = new Socket(sock);

	while (true)
	{
		std::cout << "Waiting for next msg" << std::endl;
		sock.recvHeader(msgSock, &header);
		//std::cout << "received: " << header.getID() << std::endl;
		sock.loadObj(serpi, msgSock);
		//std::cout << "llego una serpi con id: " << serpi.getSnakeID() << std::endl;
		serpi.print(std::cout);
	}

	draw(serpi, fruit);
	while (running)
	{
		sock.recvHeader(msgSock, &header);
		sock.loadObj(serpi, msgSock);
		// char in = getchar();
		// char enter = getchar();
		// switch (in)
		// {
		// case 'a':
		// 	serpi.setDir(Vector2(-1, 0));
		// 	break;
		// case 'd':
		// 	serpi.setDir(Vector2(1, 0));
		// 	break;
		// case 'w':
		// 	serpi.setDir(Vector2(0, -1));
		// 	break;
		// case 's':
		// 	serpi.setDir(Vector2(0, 1));
		// 	break;
		// case 'b':
		// 	running = false;
		// 	break;
		// default:
		// 	break;
		// }

		if (serpi.getHead() == fruit)
			serpi.increaseLength(5);
		serpi.move();
		draw(serpi, fruit);
		std::cout << i;
		i++;
	}
}
