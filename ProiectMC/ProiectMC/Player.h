#pragma once
#include <string>

class Player
{
private:
	int id;
	int x, y;
	int score;
	std::string name;
public:
	Player(int id, const std::string& name, int startX, int startY);

	void setPosition(int newX, int NewY);
	void addScore(int points);
	int getScore() const;
	void getPosition(int& outX, int& outY) const;
	void displayStatus() const;

	std::string getName() const { return name; }


};

