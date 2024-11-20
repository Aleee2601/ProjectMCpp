#pragma once
#include<iostream>
#include"map.h"
#include"Player.h"

class GameSession
{
public: 
	Map gameMap;
	std::vector<Player> players;

	GameSession(int n,int m): gameMap(n,m),currentTurn(0){}

	void addPlayer(const Player& player) { players.push_back(player); }

	void displayGameState() const;

	void playerHits(Player& shooter, Player& target);

	void nextTurn();
	Player& getCurentPlayer();
	bool isCurrentPlayerEliminated();
	void startTurn();
	void updatePlayerPosition(int playerId, int newX, int newY);
	std::vector<Player> GetAllPlayers() const;
private:
	int currentTurn;
};

