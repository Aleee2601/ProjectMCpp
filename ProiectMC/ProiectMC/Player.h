#pragma once
#include <string>
#include "Weapon.h"

enum class PlayerStatus { ACTIVE, ELIMINATED };

const int MAX_HITS = 3;
const int SCORE_FOR_UPGRADE = 500;

class Player
{
private:
	int id;
	int x, y;
	int score;
	std::string name;
	PlayerStatus status;
	int hitsTaken;
	Weapon weapon;
	
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

	void upgradeWeapon();
};

