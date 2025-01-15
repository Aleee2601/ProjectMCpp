#ifndef BULLET_H
#define BULLET_H

#include <chrono>
#include <cmath>
#include <vector>
#include <utility>

class Bullet {
private:
    double x;        // Poziția curentă pe axa X
    double y;        // Poziția curentă pe axa Y
    double dx;       // Direcția de mișcare pe X (vector normalizat)
    double dy;       // Direcția de mișcare pe Y (vector normalizat)
    double speed;    // Viteza glonțului (metri/secundă)
    bool active;     // Indicator pentru starea glonțului (dacă mai este activ)

    std::chrono::steady_clock::time_point lastMoveTime; // Timpul ultimei actualizări

public:
    Bullet(double startX, double startY, double directionX, double directionY, double bulletSpeed);

    void update(); // Actualizează poziția glonțului
    bool checkCollision(int mapWidth, int mapHeight, const std::vector<std::vector<int>>& map); // Verifică coliziunile

    bool isActive() const; // Verifică dacă glonțul este activ
    std::pair<double, double> getPosition() const; // Returnează poziția glonțului
};

#endif // BULLET_H
