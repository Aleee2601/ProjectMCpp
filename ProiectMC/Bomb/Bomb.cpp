#include "pch.h" 
#include "Bomb.h"
#include <iostream> 
#include <utility> 
Bomb::Bomb(int x, int y) : posX(x), posY(y), isActive(false) {}

Bomb::Bomb() : posX(0), posY(0), isActive(false) {}

void Bomb::Activate() {
    isActive = true;
    std::cout << "Bomb activated at (" << posX << ", " << posY << ").\n";
}

void Bomb::Deactivate() {
    isActive = false;
    std::cout << "Bomb deactivated.\n";
}

bool Bomb::IsActive() const {
    return isActive;
}

std::pair<int, int> Bomb::GetPosition() const {
    return { posX, posY };
}
