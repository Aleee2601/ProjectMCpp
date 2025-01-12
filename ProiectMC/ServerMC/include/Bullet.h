#pragma once
#include "../include/map.h"
#include "Player.h"
#include "../include/Direction.h" // Include pentru enum-ul Direction

class Bullet {
private:
    int m_x, m_y;         // Poziția curentă a glonțului
    int m_endX, m_endY;   // Poziția finală (dacă este aplicabil)
    Direction m_direction; // Direcția de deplasare a glonțului
    bool m_Active;        // Starea glonțului (activ/inactiv)

public:
    // Constructori
    Bullet(int startX, int startY, Direction dir);               // Constructor cu poziție inițială și direcție
    Bullet(int startX, int startY, int endX, int endY, Direction dir); // Constructor cu poziție inițială și finală

    // Funcții
    void Move(Map& map);                                         // Deplasează glonțul pe hartă
    bool IsOutOfBounds(int width, int height) const;             // Verifică dacă glonțul este în afara limitelor
    void LogOutOfBounds(int width, int height) const;            // Loghează dacă glonțul este în afara hărții
    bool DetectCollision(const Player& player) const;            // Verifică coliziunea cu un jucător
    bool IsInactive() const;                                     // Verifică dacă glonțul este inactiv
};
