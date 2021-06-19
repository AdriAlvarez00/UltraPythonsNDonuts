#include "SnakeGames.h"

void Vector2::to_bin(){
	_msg["x"] = x;
	_msg["y"] = y;

	std::string dump = _msg.dump();
	_json = _msg;
	_size = dump.size();
	_data = dump.c_str();
}

int Vector2::from_bin(json data){
	x = data["x"];
	y = data["y"];
	return 0;
}

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
	return 0;
}

bool GameState::collidesWithSnake(Vector2 pos){
	for(auto it = snakes.begin();it!=snakes.end();++it){
		if(it->collidesWithBody(pos))
			return true;
	}
	return false;
}

void GameState::draw(){
	printf("\033c");
		for (int y = 0; y < GRID_SIZE+2; y++)
		{
			for (int x = 0; x < GRID_SIZE+2; x++)
			{
				if (x == 0 || x == GRID_SIZE + 1 || y == 0 || y == GRID_SIZE + 1)
				{
					std::cout<<C_SNAKE;
				}
				else if (Vector2(x, y) == fruit)
				{
					std::cout<<C_FOOD;
				}
				//No es eficiente pero es debug
				else if (collidesWithSnake(Vector2(x,y)))
				{
					std::cout<<C_SNAKE;
				}
				else
				{
					std::cout<<" ";
				}
			}
			printf("\n");
		}
}

void GameState::to_bin(){
	_json["food"]["x"] = fruit.getX();
	_json["food"]["y"] = fruit.getY();
	_json["snakes"] = {};
	for(auto it = snakes.begin();it!=snakes.end();it++){
		it->to_bin();
		_json["snakes"].push_back(it->getJSON());
	}

	std::string dump = _json.dump();
	_data = dump.c_str();
	_size = dump.size();
}

int GameState::from_bin(json data){
	fruit.from_bin(data["food"]);
	
	for(int i = 0;i<snakes.size();i++)
	{
		snakes[i].from_bin(data["snakes"].at(i));
	}

	return 0;
}

void GameState::update(){
	for(auto& snake:snakes){
		snake.move(GRID_SIZE);
		if(snake.getHead() == fruit){
			snake.increaseLength(SIZE_PER_FOOD);
		}
		for(auto& checkColSnake:snakes){
			if(checkColSnake.getSnakeID()!=snake.getSnakeID()
			 && checkColSnake.collidesWithBody(snake.getHead()))
			 {
				 std::cout << "Snake " << snake.getSnakeID() << "died\n";
			 }
		}
		//TODO comprobar colision entre serpientes
	}
}
