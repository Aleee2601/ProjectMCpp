#pragma once
#include<iostream>
#include "Map.h"
#include "Player.h"
class GameSession
{
public: 
	Map gameMap;
	std::vector<Player> players;

	GameSession(int n,int m): gameMap(n,m){}

	void addPlayer(const Player& player) { players.push_back(player); }

	void displayGameState() const;

	void playerHits(Player& shooter, Player& target);
};

