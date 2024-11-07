#pragma once
#include <string>

enum class PlayerStatus { ACTIVE, ELIMINATED };

class Player
{
private:
	int id;
	int x, y;
	int score;
	std::string name;
	PlayerStatus status;
	int hitsTaken;
	const int MAX_HITS = 3;
public:
	Player(int id, const std::string& name, int startX, int startY);

	void setPosition(int newX, int NewY);
	void addScore(int points);
	int getScore() const;
	void getPosition(int& outX, int& outY) const;
	void displayStatus() const;
	void setStatus(PlayerStatus newStatus);
	PlayerStatus getStatus() const;
	void resetPosition(int startX, int startY);
	std::string getName() const { return name; }

	void takeHit();
	bool isEliminated() const;

	void addScoreForHit();


};

