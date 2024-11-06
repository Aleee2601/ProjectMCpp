#include "Player.h"
#include <iostream>

Player::Player(int id, const std::string& name, int startX, int startY)
    : id(id), name(name), x(startX), y(startY), score(0) {}

void Player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() const {
    return score;
}

void Player::getPosition(int& outX, int& outY) const {
    outX = x;
    outY = y;
}

void Player::displayStatus() const {
    std::cout << "Jucator " << id << " (" << name << ") - Pozitie: (" << x << ", " << y
        << "), Scor: " << score << std::endl;
}

void Player::setStatus(PlayerStatus newStatus)
{
    status = newStatus;
}

PlayerStatus Player::getStatus() const {
    return status;
}
