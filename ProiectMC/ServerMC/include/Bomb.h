#pragma once
#include<iostream>


class Bomb {
private:
    int posX, posY; // Poziția bombei
    bool isActive;  // Dacă bomba este activă

public:
    Bomb(int x, int y);
    ~Bomb();

    void Activate();
    void Deactivate();
    bool IsActive() const;
    std::pair<int, int> GetPosition() const;
};
