#pragma once
#include<iostream>


class Bomb {
private:
    int posX, posY; // Poziția bombei
    bool isActive;  // Dacă bomba este activă

public:
    Bomb();
    Bomb(int x, int y);

    void Activate();
    void Deactivate();
    bool IsActive() const;
    std::pair<int, int> GetPosition() const;
};
