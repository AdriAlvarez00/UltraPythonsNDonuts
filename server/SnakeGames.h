#pragma once
#include "Serializable.h"
#include <bits/stdint-intn.h>
#include <iostream>
#include <deque>
#include "json.hpp"
using json = nlohmann::json;

class Vector2 : public Serializable
{
protected:
	int x;
	int y;
	json _msg;

public:
	int getX() const { return x; }
	int getY() const { return y; }
	virtual void to_bin() override;
	virtual int from_bin(json data) override;
	Vector2 operator+(const Vector2 &b)
	{
		Vector2 v(this->x + b.x, this->y + b.y);
		return v;
	}
	bool operator==(const Vector2 &b) const
	{
		return this->x == b.x && this->y == b.y;
	}
	bool operator!=(const Vector2 &b) const
	{
		return this->x != b.x || this->y != b.y;
	}
	Vector2 operator%(const int c){
		Vector2 v(this->x%c,this->y%c);
		//TODO esto tendria q ser 0 o 1?
		if(v.x < 0) v.x = c;
		if(v.y < 0) v.y = c;
		return v;
	}

	Vector2(int _x, int _y) : x(_x), y(_y) {}
	Vector2() : x(0), y(0) {}
};

class Snake : public Serializable
{
protected:
	std::deque<Vector2> body;
	Vector2 dir;
	int32_t id;
	int32_t length;
	json _msg;

public:
	Snake() : id(0), dir(Vector2(1, 0)), body({Vector2(6, 9), Vector2(5, 9)}), length(2){};
	Snake(int i,std::vector<Vector2> body_,Vector2 dir_) : id(i), dir(dir_), body(), length(body_.size()){
		for(auto& b :body_)
			body.push_back(b);
	};
	virtual void to_bin() override;
	virtual int from_bin(json data) override;

	Vector2 getHead() const { return body.front(); }

	void print(std::ostream &os)
	{
		os << "id: " << id << " dX: " << dir.getX() << " dY: " << dir.getY() << " body: ";
		for (auto it = body.begin(); it != body.end(); ++it)
		{
			os << "x: " << it->getX() << " y: " << it->getY();
		}
	}

	void move(int gridSize)
	{
		Vector2 newHead = getHead() + dir;
		newHead = newHead%gridSize;
		body.push_front(newHead);

		if (body.size() > length)
			body.pop_back();
	}

	size_t getLenght() const{ return length;}

	void increaseLength(int32_t by)
	{
		length += by;
	}

	bool collidesWithBody(const Vector2 &v) const
	{
		auto it = body.begin();
		while (it != body.end() && *it != v)
			++it;

		return it != body.end();
	}

	void setDir(Vector2 d)
	{
		//Solo permitimos giros de 90 grados
		if((std::abs(d.getX()) == 1 && dir.getX() == 0 )||(std::abs(d.getY()) == 1 && dir.getY() == 0 ))
		dir = d;
	}

	int getSnakeID()
	{
		return id;
	}
};

class GameState : public Serializable
{
	std::vector<Snake> snakes;
	Vector2 fruit = Vector2(10,10);
	const char C_SNAKE = 'X';
	const char C_WALL = '#';
	const char C_FOOD = '0';
	const uint32_t GRID_SIZE = 20;
	const uint32_t SIZE_PER_FOOD = 2;
	bool collidesWithSnake(Vector2 pos);
public:
	GameState() {}
	virtual void to_bin() override;
	virtual int from_bin(json data) override;

	void draw();
	void moveSnake(uint32_t snakeId, Vector2 dir)
	{
		std::cout << "moviendo la serpiente con id: " << snakeId << std::endl;
		if(snakeId <= snakes.size())
			snakes[snakeId - 1].setDir(dir);
	}

	//Creamos una serpiente en un sitio nuevo
	void addSnake(Snake& snake){
		snakes.push_back(snake);
	}
	void update();
};