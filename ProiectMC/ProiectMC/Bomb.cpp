#include "Bomb.h"
#include "Map.h"

Bomb::Bomb(int x, int y) : x(x), y(y) {}

void Bomb::detonate(Map& map) {
    // Iteram prin vecini (4 celule din jur)
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {  // Evitam celula centrala
                map.destroyWall(x + i, y + j);
            }
        }
    }
}