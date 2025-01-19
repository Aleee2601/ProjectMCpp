#pragma once
#include "DBGame.h"
#include "DBPlayerWithRegion.h"
#include <vector>
#include "../Player.h"

using namespace std;

class GameDAO
{
public:
	DBGame createGame(std::vector<Player> players);
	DBGame createGame(std::vector<DBPlayerWithRegion> players);
	void updateEndedGame(DBGame game, std::vector<DBPlayerWithRegion> players);
};

