#include "SnakeGames.h"
#include "snake.pb.h"

void Vector2::fill_message(PnD::Vector2D* msg){
	msg->set_x(x);
	msg->set_y(y);
}

int Vector2::from_message(PnD::Vector2D* msg){
	x = msg->x();
	y = msg->y();
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
	std::cout << "AFTER FILL BEFORE SERIAL --------------" << std::endl;
	for(auto it = msg->body().begin(); it!=msg->body().end();++it)
		std::cout << "x: " << it->x() << "  y: " << it->y() << "///";
}

int Snake::from_message(PnD::Snake* msg){
	id = msg->id();
	std::cout << "leido id " << msg->id() << std::endl;
	std::cout << "inX: " << msg->mutable_direction()->x() << " inY: " << msg->mutable_direction()->y() << std::endl;
	dir = Vector2(msg->direction().x(),msg->direction().y());
	//TODO mejorar esto, teniendo en cuenta que las serpientes solo pueden crecer
	//Podriamos actualizar el cuerpo existente, comprobar el size y solo hacer resize si es necesario
	body.resize(msg->body_size());
	std::cout << "leyendo cuerpo tam: " << msg->body_size() << std::endl;
	for(int i = 0;i<msg->body_size();i++){
		body[i].from_message((PnD::Vector2D*)&msg->body(i));
	}

	return 0;
}
