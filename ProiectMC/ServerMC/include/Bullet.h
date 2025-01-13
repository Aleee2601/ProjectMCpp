#pragma once

#include "Direction.h"
#include "Map.h"

// Clasa Bullet reprezintă un glonț tras de un jucător
class Bullet {
private:
    int m_x, m_y;          // Poziția curentă a glonțului
    Direction m_direction; // Direcția glonțului
    bool m_active;         // Starea glonțului (activ/inactiv)

public:
    // Constructor: Inițializează glonțul cu poziția de start și direcția
    Bullet(int startX, int startY, Direction dir);

    // Mută glonțul în direcția curentă
    void Move(Map& map);

    // Returnează poziția curentă a glonțului
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }
    Direction GetDirection() const { return m_direction; }
};
