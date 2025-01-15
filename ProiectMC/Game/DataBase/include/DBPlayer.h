#pragma once
#include <string>

class DBPlayer
{
public:
	int player_id;
	std::string nickname;
	std::string password;
	int points;
	int score;

	DBPlayer();
	DBPlayer(std::nullptr_t);
	bool operator==(std::nullptr_t);


};