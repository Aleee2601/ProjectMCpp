#include "../include/Bomb.h"
#include<iostream>

Bomb::Bomb(int x, int y) : posX(x), posY(y), isActive(false) {}
Bomb::Bomb() : posX(0), posY(0), isActive(false) {}

void Bomb::Activate() {
    isActive = true;
}

void Bomb::Deactivate() {
    isActive = false;
}

bool Bomb::IsActive() const {
    return isActive;
}

std::pair<int, int> Bomb::GetPosition() const {
    return { posX, posY };
}
