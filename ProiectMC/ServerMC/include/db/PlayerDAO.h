#pragma once
#include "DBPlayer.h"

class PlayerDAO
{
public:
	DBPlayer insertPlayer(std::string nickname, std::string password);
	DBPlayer loginPlayer(std::string nickname, std::string password);
	DBPlayer findPlayerByNickname(std::string nickname);
	DBPlayer findPlayerById(int id);
	void updatePlayer(DBPlayer& player);
	void updatePlayerScore(int idPlayer, int score);
};
