#include "snake.pb.h"
#include "SnakeGames.h"
#include "iostream"
#include "fstream"
#include <fstream>

int main(){
	PnD::Snake *serpi = new PnD::Snake();
	serpi->set_id(12);
	auto v1 = serpi->add_body();
	v1->set_x(10);
	v1->set_y(12);
	PnD::Vector2D *dir = new PnD::Vector2D();
	dir->set_x(1);
	dir->set_y(0);
	serpi->set_allocated_direction(dir);

	Vector2 myVec(3,5);
	myVec.to_bin();

	std::ofstream file;
	file.open("v.data");
	file << myVec.data();
	file.close();
	std::cout << "x: " << myVec.getX() << "y: " << myVec.getY() << std::endl;

	std::ifstream fileIn;
	std::string buf;
	fileIn.open("v.data");
	fileIn >> buf;
	Vector2 recV(0,0);
	std::cout << "x: " << recV.getX() << "y: " << recV.getY() << std::endl;
	recV.from_bin((char*)buf.c_str());
	std::cout << "x: " << recV.getX() << "y: " << recV.getY() << std::endl;
}
