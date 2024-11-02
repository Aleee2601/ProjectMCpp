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

	void displayGameState() const {
		std::cout << "Current Game State:\n";
		for (const auto& player : players) {
			int x, y;
			player.getPosition(x, y);
			std::cout << "Player" << player.getName()
				<< "is at position (" << x << "," << y << ")\n";
		}
	}
};

