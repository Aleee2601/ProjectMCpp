#pragma once
enum class Direction { Up, DOWN, LEFT, RIGHT };
class Bullet
{
public:
	int x, y;
	Direction direction;
	Bullet(int startX, int startY, Direction dir) :x(startX), y(startY), direction(dir){}
};

