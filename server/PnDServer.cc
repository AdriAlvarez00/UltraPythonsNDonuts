#include "snake.pb.h"
#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>

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
}
