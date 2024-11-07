module player;
#include <iostream>

Player::Player(int id, const std::string& name, int startX, int startY)
    : id(id), name(name), x(startX), y(startY), score(0), status(PlayerStatus::ACTIVE) {}

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

void Player::resetPosition(int startX, int startY) {
    x = startX;
    y = startY;
}

void Player::takeHit() {
    if (status == PlayerStatus::ACTIVE)
        hitsTaken++;
}

bool Player::isEliminated()const {
    return hitsTaken >= MAX_HITS;
}
