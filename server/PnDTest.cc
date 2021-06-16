#include "snake.pb.h"
#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>
#include <stdlib.h> 


#define GRID_SIZE 20
#define SNAKE X
#define WALL #

void draw(const Snake& snake,const Vector2& fruit){
	printf("\033c");
	for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (x == 0 || x == GRID_SIZE - 1 || y == 0
                || y == GRID_SIZE - 1) {
                printf("#");
            }
			else if(Vector2(x,y)==fruit){
				printf("*");
			}
			//No es eficiente pero es debug
			else if(snake.collidesWithBody(Vector2(x,y))){
				printf("O");
			}
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main(){


	Snake serpi(1);
	serpi.to_bin();

	std::ofstream file;
	std::ifstream fileIn;
	file.open("snake.data");
	PnD::Snake* msg = serpi.get_message();
	int siz = msg->ByteSizeLong();
	msg->SerializeToOstream(&file);
	file.close();

	fileIn.open("snake.data");
	std::string buf2;
	fileIn >> buf2;
	serpi.print(std::cout);
	std::cout << std::endl;
	Snake serpi2(2);
	PnD::Snake* aux = new PnD::Snake();
	aux->ParsePartialFromIstream(&fileIn);
	serpi2.from_message(aux);
	serpi2.print(std::cout);

	bool running=true;
	Vector2 fruit(5,5);
	draw(serpi,fruit);
	int i = 0;
	while(running){
		char in = getchar();
		char enter = getchar();
		switch (in)
		{
		case 'a':
			serpi.setDir(Vector2(-1,0));
			break;
		case 'd':
			serpi.setDir(Vector2(1,0));
			break;
		case 'w':
			serpi.setDir(Vector2(0,-1));
			break;
		case 's':
			serpi.setDir(Vector2(0,1));
			break;
		case 'b':
			running = false;
			break;
		default:
			break;
		}

		if(serpi.getHead()==fruit)
			serpi.increaseLength(5);
		serpi.move();
		draw(serpi,fruit);
		std::cout << i;
		i++;
	}
}
