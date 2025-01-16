#include "Player.h"
#include <stdexcept>
#include <regex>
#include <iostream>

Player::Player()
    : m_username("Unknown"), m_x(1), m_y(1),
    m_spawnX(1), m_spawnY(1),
    m_remainingLives(3), m_points(0),
    m_reloadTime(4), m_bulletSpeed(0.25),
    m_lastShot(std::chrono::steady_clock::now()),
    m_directionX(0), m_directionY(1) {
    // Default initialization
}

Player::Player(const std::string& username, int startX = 1, int startY = 1)
    : m_username(username), m_x(startX), m_y(startY),
    m_spawnX(startX), m_spawnY(startY),
    m_remainingLives(3), m_points(0),
    m_reloadTime(4), m_bulletSpeed(0.25),
    m_lastShot(std::chrono::steady_clock::now()),
    m_directionX(0), m_directionY(1) {
    // Validate username
    if (!std::regex_match(username, std::regex("[A-Za-z0-9_]{3,16}"))) {
        throw std::invalid_argument("Invalid username. Must be 3-16 alphanumeric characters.");
    }
}


Player::Player(const Player& other)
    : m_username(other.m_username), m_x(other.m_x), m_y(other.m_y),
    m_spawnX(other.m_spawnX), m_spawnY(other.m_spawnY),
    m_remainingLives(other.m_remainingLives), m_points(other.m_points),
    m_reloadTime(other.m_reloadTime), m_bulletSpeed(other.m_bulletSpeed),
    m_lastShot(other.m_lastShot),
    m_directionX(other.m_directionX), m_directionY(other.m_directionY) {
}

Player::Player(Player&& other) noexcept
    : m_username(std::move(other.m_username)), m_x(other.m_x), m_y(other.m_y),
    m_spawnX(other.m_spawnX), m_spawnY(other.m_spawnY),
    m_remainingLives(other.m_remainingLives), m_points(other.m_points),
    m_reloadTime(other.m_reloadTime), m_bulletSpeed(other.m_bulletSpeed),
    m_lastShot(std::move(other.m_lastShot)),
    m_directionX(other.m_directionX), m_directionY(other.m_directionY) {
    other.m_x = 0;
    other.m_y = 0;
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        m_username = other.m_username;
        m_x = other.m_x;
        m_y = other.m_y;
        m_spawnX = other.m_spawnX;
        m_spawnY = other.m_spawnY;
        m_remainingLives = other.m_remainingLives;
        m_points = other.m_points;
        m_reloadTime = other.m_reloadTime;
        m_bulletSpeed = other.m_bulletSpeed;
        m_lastShot = other.m_lastShot;
        m_directionX = other.m_directionX;
        m_directionY = other.m_directionY;
    }
    return *this;
}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        m_username = std::move(other.m_username);
        m_x = other.m_x;
        m_y = other.m_y;
        m_spawnX = other.m_spawnX;
        m_spawnY = other.m_spawnY;
        m_remainingLives = other.m_remainingLives;
        m_points = other.m_points;
        m_reloadTime = other.m_reloadTime;
        m_bulletSpeed = other.m_bulletSpeed;
        m_lastShot = std::move(other.m_lastShot);
        m_directionX = other.m_directionX;
        m_directionY = other.m_directionY;
    }
    return *this;
}

void Player::move(int dx, int dy) {
    m_x += dx;
    m_y += dy;
}

void Player::registerHit() {
    if (m_remainingLives > 0) {
        m_remainingLives--;
    }
    if (m_remainingLives == 0) {
        resetPosition();
    }
}

void Player::resetPosition() {
    m_x = m_spawnX;
    m_y = m_spawnY;
}

void Player::addPoints(int points) {
    m_points += points;
}

bool Player::canShoot() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastShot).count();
    return elapsed >= m_reloadTime;
}

void Player::recordShot() {
    m_lastShot = std::chrono::steady_clock::now();
}
