#pragma once
#include <vector>
#include <functional> // Pentru std::function

class Bomb
{
public:
    // Constructor: Initializes the bomb's position.
    Bomb(int x, int y);

    // Detonates the bomb using a callback to destroy walls
    void Detonate(std::function<void(int, int)> destroyWallCallback);
   
    bool IsInactive();
    // Calculates explosion effects without direct references to Map or Player
    void CalculateExplosionEffects(
        std::function<bool(int, int)> isWallDestructibleCallback,
        std::function<void(int, int)> destroyWallCallback,
        std::function<void(int, int)> applyDamageToPlayerCallback
    );
private:
    // Bomb's position on the x-axis.
    int m_x;

    // Bomb's position on the y-axis.
    int m_y;
};
