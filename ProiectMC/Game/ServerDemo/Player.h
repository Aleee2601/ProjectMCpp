#pragma once

#include <string>
#include <chrono>

class Player {
public:
    Player();
    // Constructor
    Player(const std::string& username, int startX, int startY);

    // Copy constructor
    Player(const Player& other);

    // Copy assignment operator
    Player& operator=(const Player& other);

    // Move constructor
    Player(Player&& other) noexcept;

    // Move assignment operator
    Player& operator=(Player&& other) noexcept;

    // Destructor
    ~Player() = default;

    // Position getters
    int getX() const { return m_x; }
    int getY() const { return m_y; }

    // Position manipulation
    void moveTo(int newX, int newY) { m_x = newX; m_y = newY; }
    void move(int dx, int dy);

    // Player information
    std::string getName() const { return m_username; }
    int getRemainingLives() const { return m_remainingLives; }
    void registerHit();
    void resetPosition();
    void addPoints(int points);

    // Shooting logic
    bool canShoot() const;
    void recordShot();

    // Direction
    void setDirection(int dx, int dy) { m_directionX = dx; m_directionY = dy; }
    int getDirectionX() const { return m_directionX; }
    int getDirectionY() const { return m_directionY; }

private:
    std::string m_username;
    int m_x;
    int m_y;
    int m_spawnX;
    int m_spawnY;
    int m_remainingLives;
    int m_points;
    int m_reloadTime;
    double m_bulletSpeed;
    std::chrono::steady_clock::time_point m_lastShot;
    int m_directionX;
    int m_directionY;
};
