#include "Bullet.h"

Bullet::Bullet(double startX, double startY, double directionX, double directionY, double bulletSpeed)
    : x(startX), y(startY), speed(bulletSpeed), active(true)
{
    // Normalizăm direcția
    double magnitude = std::sqrt(directionX * directionX + directionY * directionY);
    dx = directionX / magnitude;
    dy = directionY / magnitude;

    // Inițializăm timpul
    lastMoveTime = std::chrono::steady_clock::now();
}

void Bullet::update() {
    if (!active) return;

    // Calculăm timpul scurs de la ultima actualizare
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = now - lastMoveTime;
    lastMoveTime = now;

    // Actualizăm poziția pe baza vitezei și direcției
    x += dx * speed * elapsedSeconds.count();
    y += dy * speed * elapsedSeconds.count();
}

bool Bullet::checkCollision(int mapWidth, int mapHeight, const std::vector<std::vector<int>>& map) {
    if (!active) return false;

    int mapX = static_cast<int>(x);
    int mapY = static_cast<int>(y);

    // Verificăm limitele hărții
    if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) {
        active = false; // Dezactivăm glonțul
        return true;
    }

    // Verificăm coliziunea cu un obstacol (de exemplu, 1 = obstacol)
    if (map[mapY][mapX] == 1) {
        active = false; // Dezactivăm glonțul
        return true;
    }

    return false;
}

bool Bullet::isActive() const {
    return active;
}

std::pair<double, double> Bullet::getPosition() const {
    return { x, y };
}
