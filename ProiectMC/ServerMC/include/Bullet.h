#pragma once
#include "Direction.h"
#include "Map.h"

class Bullet {
private:
    int m_x, m_y;          // Poziția curentă
    Direction m_direction; // Direcția de deplasare
    bool m_active;         // Starea glonțului (activ/inactiv)

public:
    Bullet(int startX, int startY, Direction dir);

    void Move(Map& map);    // Deplasează glonțul pe hartă
    bool IsInactive() const; // Verifică dacă glonțul este inactiv
};
