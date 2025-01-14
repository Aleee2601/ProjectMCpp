#pragma once
#include "Direction.h"


// Clasa Bullet reprezintă un glonț tras de un jucător
class Bullet {
private:
    int m_x, m_y;          // Poziția curentă a glonțului
    Direction m_direction; // Direcția glonțului
    bool m_active;         // Starea glonțului (activ/inactiv)
    int m_ownerId;         // ID-ul jucătorului care a tras glonțul

public:
    Bullet(int startX, int startY, Direction dir, int ownerId)
        : m_x(startX), m_y(startY), m_direction(dir), m_active(true), m_ownerId(ownerId) {
    }
    bool IsInactive() const { return !m_active; }
    // Verifică dacă glonțul este inactiv

    // Returnează poziția curentă a glonțului
    void SetInactive() { m_active = false; }
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }
    Direction GetDirection() const { return m_direction; }
    int GetOwnerId() const { return m_ownerId; } // Returnează ID-ul jucătorului care a tras glonțul
};
