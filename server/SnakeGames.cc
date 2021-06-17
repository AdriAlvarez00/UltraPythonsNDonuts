#include "SnakeGames.h"
#include "snake.pb.h"

void Vector2::to_bin(){
	_msg.set_x(x);
	_msg.set_y(y);
	_data = _msg.SerializeAsString();
	_size = _msg.ByteSizeLong();
}

int Vector2::from_bin(char* data){
	_msg.ParseFromString(data);
	x = _msg.x();
	y = _msg.y();
	return 0;
}

void Snake::to_bin(){
	
	_msg.set_id(id);
	std::cout << "filled id: " << _msg.id() << std::endl;
	_msg.mutable_direction()->set_x(dir.getX());
	_msg.mutable_direction()->set_y(dir.getY());
	std::cout << _msg.direction().x() << " " << _msg.direction().y() << std::endl;
	_msg.clear_body();
	for(auto it = body.begin();it!=body.end();++it){
		PnD::Vector2D* v = _msg.add_body();
		v->set_x(it->getX());
		v->set_y(it->getY());
	}
	std::cout << "AFTER FILL BEFORE SERIAL --------------" << std::endl;
	for(auto it = _msg.body().begin(); it!=_msg.body().end();++it)
		std::cout << "x: " << it->x() << "  y: " << it->y() << "///";

	_data = _msg.SerializeAsString();
	_size = _msg.ByteSizeLong();
}

int Snake::from_bin(char* data){
	_msg.ParseFromString(data);
	id = _msg.id();
	std::cout << "leido id " << _msg.id() << std::endl;
	std::cout << "inX: " << _msg.mutable_direction()->x() << " inY: " << _msg.mutable_direction()->y() << std::endl;
	dir = Vector2(_msg.direction().x(),_msg.direction().y());
	//TODO mejorar esto, teniendo en cuenta que las serpientes solo pueden crecer
	//Podriamos actualizar el cuerpo existente, comprobar el size y solo hacer resize si es necesario
	body.resize(_msg.body_size());
	std::cout << "leyendo cuerpo tam: " << _msg.body_size() << std::endl;
	for(int i = 0;i<_msg.body_size();i++){
		body[i] = Vector2(_msg.body()[i].x(),_msg.body()[i].y());
	}

	return 0;
}
