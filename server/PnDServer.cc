#include "snake.pb.h"
#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>

int main(){

	Vector2 myVec(3,5);
	myVec.to_bin();

	std::ofstream file;
	file.open("v.data");
	file << myVec.data();
	file.close();
	//std::cout << "x: " << myVec.getX() << "y: " << myVec.getY() << std::endl;

	std::ifstream fileIn;
	std::string buf;
	fileIn.open("v.data");
	fileIn >> buf;
	Vector2 recV(0,0);
	std::cout << "x: " << recV.getX() << "y: " << recV.getY() << std::endl;
	recV.from_bin((char*)buf.c_str());
	std::cout << "x: " << recV.getX() << "y: " << recV.getY() << std::endl;


	Snake serpi(1);
	serpi.to_bin();

	file.open("snake.data");
	file << serpi.data();
	file.close();

	fileIn.open("snake.data");
	std::string buf2;
	fileIn >> buf2;
	serpi.print(std::cout);
	std::cout << std::endl;
	Snake serpi2(2);
	serpi2.from_bin((char*)buf.c_str());
	serpi2.print(std::cout);
}
