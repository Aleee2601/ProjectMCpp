#pragma once
#include<vector>
#include "Player.h"
class Map;

class Bomb
{
public:
	Bomb(int x, int y);
	void detonate(Map& map);
	void calculateExplosionEffects(Map& map, std::vector<Player>& players);
private:
	int x, y;
};

