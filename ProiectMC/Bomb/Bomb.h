#pragma once

#ifdef BOMB_EXPORTS
#define BOMB_API __declspec(dllexport)
#else
#define BOMB_API __declspec(dllimport)
#endif

#include <utility>

class BOMB_API Bomb {
private:
    int posX;
    int posY;
    bool isActive;

public:
    Bomb(int x, int y);
    Bomb();

    void Activate();
    void Deactivate();
    bool IsActive() const;
    std::pair<int, int> GetPosition() const;
};