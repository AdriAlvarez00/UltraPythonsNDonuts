#include "Serializable.h"
#include "snake.pb.h"
#include <bits/stdint-intn.h>
#include <iostream>
#include <deque>

class Header : public Serializable{
	public:
	PnD::MessageID id;
	PnD::Header _msgHeader;
	virtual void to_bin() override{
		_msgHeader.set_msgid(id);
		_data = _msgHeader.SerializeAsString();
		_size = _msgHeader.ByteSizeLong();
	}
	virtual int from_bin(char* data) override{
		_msgHeader.ParseFromString(data);
		id = _msgHeader.msgid();
		return 0;
	}

	Header(PnD::MessageID i=PnD::MessageID::LOGINPETITION):id(i){
	}

};

class Vector2 : public Serializable{
	protected:
		int x;
		int y;
		PnD::Vector2D _msg;
	public:
		int getX() const {return x;}
		int getY() const {return y;}
		virtual void to_bin() override;
		virtual int from_bin(char* bin) override;
		Vector2 operator+(const Vector2& b){
			Vector2 v(this->x+b.x,this->y+b.y);
			return v;
		}
		bool operator==(const Vector2& b) const{
			return this->x == b.x && this->y==b.y;
		}
		bool operator!=(const Vector2& b) const{
			return this->x != b.x || this->y!=b.y;
		}

		Vector2(int _x, int _y):x(_x),y(_y){}
		Vector2():x(0),y(0){}


};

class Snake : public Serializable{
	protected:
		std::deque<Vector2> body;
		Vector2 dir;
		int32_t id;
		int32_t length;
		PnD::Snake _msg;
	public:
		Snake(int i):id(i),dir(Vector2(1,0)),body({Vector2(6,9),Vector2(5,9)}),length(2){};
		virtual void to_bin() override;
		virtual int from_bin(char* data) override;

		Vector2 getHead() const{return body.front();}

		void print(std::ostream& os){
			os << "id: " << id << " dX: " << dir.getX() << " dY: " << dir.getY() << " body: ";
			for(auto it = body.begin();it!=body.end();++it)
			{
			os << "x: " << it->getX() << " y: " << it->getY();
			}
		}

		void move(){
			Vector2 newHead = getHead() + dir;
			body.push_front(newHead);

			if(body.size()>length)
				body.pop_back();
		}

		void increaseLength(int32_t by){
			length += by;
		}

		bool collidesWithBody(const Vector2& v) const{
			auto it = body.begin();
			while(it!=body.end() && *it!=v)
				++it;

			return it != body.end();			
		}

		void setDir(Vector2 d){
			//TODO validacion de input
			dir=d;
		}

		int getSnakeID(){
			return id;
		}
};
