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
