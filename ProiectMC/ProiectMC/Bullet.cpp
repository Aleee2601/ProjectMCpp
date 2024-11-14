#include "Bullet.h"
#include <iostream>
#include <unordered_map>

Bullet::Bullet(int startX, int startY, Direction dir)
    : x(startX), y(startY), direction(dir), endX(0), endY(0) {}

Bullet::Bullet(int startX, int startY, int endX, int endY, Direction dir)
    : x(startX), y(startY), endX(endX), endY(endY), direction(dir) {}

void Bullet::move() {
    static const std::unordered_map<Direction, std::pair<int, int>> directionOffsets = {
        {Direction::UP, {0, -1}},
        {Direction::DOWN, {0, 1}},
        {Direction::LEFT, {-1, 0}},
        {Direction::RIGHT, {1, 0}}
    };
    x += directionOffsets.at(direction).first;
    y += directionOffsets.at(direction).second;
}

bool Bullet::isOutOfBounds(int width, int height) const {
    return (x < 0 || x >= width || y < 0 || y >= height);
}

void Bullet::logOutOfBounds(int width, int height) const {
    if (isOutOfBounds(width, height)) {
        std::cout << "Bullet out of bounds at position: (" << x << "," << y << ")\n";
    }
}

bool Bullet::detectCollision(const Player& player) const {
    int playerX, playerY;
    player.getPosition(playerX, playerY);
    return (x == playerX && y == playerY);
}
