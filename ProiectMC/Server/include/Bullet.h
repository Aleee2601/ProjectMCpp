#pragma once
#include <unordered_map>
#include <iostream>
#include "map.h"
#include "Player.h" // Included for the detectCollision method

// Enum for bullet movement directions
enum class Direction { UP, DOWN, LEFT, RIGHT };

class Bullet {
private:
    int m_x;           // Bullet's current position on the x-axis
    int m_y;           // Bullet's current position on the y-axis
    Direction m_direction; // Direction of bullet movement
    int m_endX;        // Target position on the x-axis (optional)
    int m_endY;        // Target position on the y-axis (optional)

public:
    // Constructor for a bullet with a starting position and direction
    Bullet(int startX, int startY, Direction dir);

    // Constructor for a bullet with a starting and ending position, plus direction
    Bullet(int startX, int startY, int endX, int endY, Direction dir);

    // Moves the bullet one step in its direction
    void Move(Map &map);

    // Checks if the bullet is out of the map bounds
    bool IsOutOfBounds(int width, int height) const;

    // Logs a message if the bullet is out of bounds
    void LogOutOfBounds(int width, int height) const;

    // Detects collision with a player
    bool DetectCollision(const Player& player) const;
};
