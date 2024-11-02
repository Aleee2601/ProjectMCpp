#pragma once
#include "Map.h"
#include "Player.h"
class GameSession
{
public: 
	Map gameMap;
	std::vector<Player> players;

	GameSession(int n,int m): gameMap(n,m){}

};

