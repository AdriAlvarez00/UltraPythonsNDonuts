#include "Serializable.h"
#include "snake.pb.h"
#include <bits/stdint-intn.h>
#include <iostream>

class Vector2 : public Serializable<PnD::Vector2D>{
	protected:
		int x;
		int y;
	public:
		int getX() const {return x;}
		int getY() const {return y;}
		virtual void fill_message(PnD::Vector2D* msg) override;
		virtual int from_message(PnD::Vector2D* msg) override;
		Vector2(int _x, int _y):x(_x),y(_y){}
		Vector2():x(0),y(0){}


};

class Snake : public Serializable<PnD::Snake>{
	protected:
		std::vector<Vector2> body;
		Vector2 dir;
		int32_t id;
	public:
		Snake(int i):id(i),dir(Vector2(1,0)),body({Vector2(6,9),Vector2(5,9)}){};
		virtual void fill_message(PnD::Snake* msg) override;
		virtual int from_message(PnD::Snake* msg) override;

		void print(std::ostream& os){
			os << "id: " << id << " dX: " << dir.getX() << " dY: " << dir.getY() << " body: ";
			for(auto it = body.begin();it!=body.end();++it)
			{
			os << "x: " << it->getX() << " y: " << it->getY();
			}
		}
};
