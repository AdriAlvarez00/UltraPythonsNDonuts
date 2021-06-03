#include "SnakeGames.h"
#include "snake.pb.h"


void Vector2::to_bin(){
	fill_message(&msg);
	_data = msg.SerializeAsString();
	_size = msg.ByteSizeLong();
}


int Vector2::from_bin(char* c){
	msg.ParsePartialFromString(c);
	return from_message(&msg);
}

void Vector2::fill_message(PnD::Vector2D* msg){
	msg->set_x(x);
	msg->set_y(y);
}

int Vector2::from_message(PnD::Vector2D* msg){
	x = msg->x();
	y = msg->y();
	return 0;
}



void Snake::to_bin(){
	fill_message(&msg);
	_data = msg.SerializeAsString();
	_size = msg.ByteSizeLong();
}

int Snake::from_bin(char* c){
	msg.ParseFromString(c);
	from_message(&msg);
	return 0;
}

void Snake::fill_message(PnD::Snake* msg){
	msg->set_id(id);
	std::cout << "filled id: " << msg->id() << std::endl;
	dir.fill_message(msg->mutable_direction());
	std::cout << msg->direction().x() << " " << msg->direction().y() << std::endl;
	msg->clear_body();
	for(auto it = body.begin();it!=body.end();++it){
		it->fill_message(msg->add_body());
	}
	if(body.size() == msg->body_size())
		std::cout << "ben inisiao" << std::endl;
}

int Snake::from_message(PnD::Snake* msg){
	id = msg->id();
	std::cout << "leido id " << msg->id() << std::endl;
	std::cout << "inX: " << msg->mutable_direction()->x() << " inY: " << msg->mutable_direction()->y() << std::endl;
	dir = Vector2(msg->direction().x(),msg->direction().y());
	body.resize(msg->body_size());
	std::cout << "leyendo cuerpo tam: " << msg->body_size() << std::endl;
	for(int i = 0;i<msg->body_size();i++){
		body[i].from_message((PnD::Vector2D*)&msg->body(i));
	}

	return 0;
}
