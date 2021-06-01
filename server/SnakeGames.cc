#include "SnakeGames.h"
#include "snake.pb.h"


void Vector2::to_bin(){
	msg.set_x(x);
	msg.set_y(y);
	_data = msg.SerializeAsString();
	_size = msg.ByteSizeLong();
}


int Vector2::from_bin(char* c){
	msg.ParsePartialFromString(c);
	x = msg.x();
	y = msg.y();

	return 1;
}
