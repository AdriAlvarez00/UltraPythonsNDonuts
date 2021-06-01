#include "Serializable.h"
#include "snake.pb.h"

class Vector2 : public Serializable{
	protected:
		int x;
		int y;
		PnD::Vector2D msg;
	public:
		int getX(){return x;}
		int getY(){return y;}
		void to_bin() override;
		int from_bin(char *data) override;
		Vector2(int _x, int _y):x(_x),y(_y){}
		Vector2():x(0),y(0){}
};
