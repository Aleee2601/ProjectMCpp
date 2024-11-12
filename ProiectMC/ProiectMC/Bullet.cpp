#pragma once
#include<iostream>
#include<unordered_map>
import player;

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Bullet
{
private:
	int x, y;
	Direction direction;
	int endX, endY;

public:
	Bullet(int startX, int startY, Direction dir) :x(startX), y(startY), direction(dir) {}

	//adaug constructor care ma va ajuta sa verific pozitia finala a glontului
	Bullet(int startX, int startY, int endX, int endY, Direction dir)
		: x(startX), y(startY), endX(endX), endY(endY), direction(dir) {}

	//se utilizeaza o harta pentru a stoca offset-urile asociate fiecarei directii posibile
	void move() {
		static const std::unordered_map<Direction, std::pair<int, int>> directionOffsets = {
			{Direction::UP, {0, -1}},
			{Direction::DOWN, {0, 1}},
			{Direction::LEFT, {-1, 0}},
			{Direction::RIGHT, {1, 0}}
		};
		x += directionOffsets.at(direction).first;
		y += directionOffsets.at(direction).second;
	}

	//verifica daca glontul a iesit din limitele ariei de joc
	bool isOutOfBounds(int width, int height) const {
		return (x < 0 || x >= width || y < 0 || y >= height);
	}

	//afiseaza un mesaj in consola daca glontul a iesit din limitele ariei
	void logOutOfBounds(int width, int height) const {
		if (isOutOfBounds(width, height)) {
			std::cout << "Bullet out of bounds at position: (" << x << "," << y << ")\n";
		}
	}

	// verifica daca un clont a lovit un jucator
	bool detectCollision(const Player& player) const {
		int playerX, playerY;
		player.getPosition(playerX, playerY);  
		return (x == playerX && y == playerY); 
	}



};

