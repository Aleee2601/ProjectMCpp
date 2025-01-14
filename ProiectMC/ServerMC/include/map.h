#pragma once
#include "Bomb.h"
#include "Player.h"
#include <vector>
#include <iostream>
#include <random>

// Enum for different types of cells on the map
enum class CellType {
    EMPTY,              // Cell is empty and can be traversed
    DESTRUCTIBLE_WALL,  // Cell contains a destructible wall
    INDESTRUCTIBLE_WALL // Cell contains an indestructible wall
};

class Map {
private:
    std::vector<std::vector<CellType>> m_grid; // 2D grid representing the map
    int m_width;                               // Map width
    int m_height;                              // Map height
    std::vector<Bomb> m_bombs;     // List of bombs on the map

public:
    // Constructor: Initializes the map with the given dimensions and empty cells
    Map(int n, int m);     // Constructor cu parametri
    Map();                 // Constructor implicit

    // Returns the type of cell at the specified coordinates
    CellType GetCellType(int x, int y) const;
    // Returns the map's width
    int GetWidth() const;

    // Returns the map's height
    int GetHeight() const;

    // Sets the type of cell at the specified coordinates
    void SetCellType(int x, int y, CellType type);

    // Generates a random map with destructible and indestructible walls
    void GenerateRandomMap();

    void DestroyWall(int x, int y);

    void DestroyWallWithDisplay(int x, int y, std::vector<Player>& players);

    // Displays the map in the console for debugging purposes
    void DisplayMap() const;

    void HandleBombEffect(int bombX, int bombY, std::vector<Player>& players);

    void ActivateBombIfNeeded(int x, int y, std::vector<Player>& players);

    // Returns a list of empty cells
    std::vector<std::pair<int, int>> GetEmptyCells() const;

    // Checks if the specified coordinates are within the bounds of the map
    bool IsWithinBounds(int x, int y) const;

    void CheckBulletCollisions(std::vector<Player>& players, std::vector<Bullet>& bullets);

    bool IsCollisionWithWall(int x, int y) const;

    void RemoveInactiveBombs();
};