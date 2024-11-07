#pragma once
#include <unordered_map>
#include <iostream>
module player; // am inclus pt a putea face metoda detectCollision

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Bullet
{
private:
    int x, y;                 
    Direction direction;      
    int endX, endY;           

public:
    
    Bullet(int startX, int startY, Direction dir);
    Bullet(int startX, int startY, int endX, int endY, Direction dir);

    
    void move();
    
    bool isOutOfBounds(int width, int height) const;

    void logOutOfBounds(int width, int height) const;

    bool detectCollision(const Player& player) const;

};

