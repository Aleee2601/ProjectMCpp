#pragma once
class Map;
class Bomb
{
public:
	Bomb(int x, int y);
	void detonate(Map& map);
private:
	int x, y;
};

