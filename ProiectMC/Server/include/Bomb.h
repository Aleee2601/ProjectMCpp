#pragma once
#include <vector>
#include "Player.h"

class Map;
class Player;

class Bomb
{
public:
    // Constructor: Initializes the bomb's position.
    Bomb(int x, int y);

    // Detonates the bomb, causing changes to the map.
    void Detonate(Map& map);

    // Calculates the effects of the bomb's explosion on the map and players.
    void CalculateExplosionEffects(Map& map, std::vector<Player>& players);

private:
    // Bomb's position on the x-axis.
    int m_x;

    // Bomb's position on the y-axis.
    int m_y;
};
