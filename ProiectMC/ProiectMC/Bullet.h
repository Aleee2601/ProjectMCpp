#pragma once
#include<iostream>
enum class Direction { UP, DOWN, LEFT, RIGHT };
class Bullet
{
private:
	int x, y;
	Direction direction;
public:
	Bullet(int startX, int startY, Direction dir) :x(startX), y(startY), direction(dir){}
	void move() {
		switch (direction) {
		case Direction::UP: --y; break;
		case Direction::DOWN: ++y; break;
		case Direction::LEFT: --x; break;
		case Direction::RIGHT: ++x; break;
		}
	}
	bool isOutOfBounds(int width, int height) const {
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			std::cout << "Bullet out of bounds at position: ("
				<< x << "," << y << ")\n";
			return true;
		}

		return false;
	}
};

