#include "SnakeGames.h"

void Vector2::to_bin(){
}

int Vector2::from_bin(json data){}

void Snake::to_bin(){
	_msg["playerId"] = id;
	_msg["direction"]["x"] = dir.getX();
	_msg["direction"]["y"] = dir.getY();
	_msg["body"] = {};
	for(auto it = body.begin();it!= body.end();it++){
		json v2;
		v2["x"] = it->getX();
		v2["y"] = it->getY();
		_msg["body"].push_back(v2);
	}
	_msg["length"] = length;

	std::string dump = _msg.dump();
	_json = _msg;
	_size = dump.size();
	_data = dump.c_str();
}

int Snake::from_bin(json data){
	id = data["playerId"];
	dir = Vector2(data["direction"]["x"],data["direction"]["y"]);
	body.clear();
	for(auto it=data["body"].begin();it!=data["body"].end();++it){
		body.push_back(Vector2((*it)["x"].get<int32_t>(),(*it)["y"].get<int32_t>()));
	}
	length = data["length"];
}
