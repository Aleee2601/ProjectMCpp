#pragma once
#include<iostream>


// Enum for bullet directions
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


// Funcție utilitară pentru calcularea poziției următoare
inline std::pair<int, int> GetNextPosition(int x, int y, Direction direction) {
    switch (direction) {
    case Direction::UP:    return { x, y - 1 };
    case Direction::DOWN:  return { x, y + 1 };
    case Direction::LEFT:  return { x - 1, y };
    case Direction::RIGHT: return { x + 1, y };
    default:               return { x, y }; // Dacă direcția nu este validă
    }
}