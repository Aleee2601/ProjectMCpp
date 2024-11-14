#include "map.h"


import <random>;

// Constructor that initializes the map with empty spaces and random configuration
Map::Map(int n, int m) : width(n), height(m), grid(n, std::vector<CellType>(m, CellType::EMPTY)) {
    generateRandomMap();
}

// Returns the type of cell at coordinates (x, y)
CellType Map::getCellType(int x, int y) const {
    return grid.at(x).at(y);
}

// Sets the type of cell at coordinates (x, y), ensuring coordinates are within bounds
void Map::setCellType(int x, int y, CellType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[x][y] = type;
    }
}

// Generates a random map with destructible and indestructible walls
void Map::generateRandomMap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int randValue = dist(gen);
            if (randValue < 70) {
                grid[i][j] = CellType::EMPTY;
            }
            else if (randValue < 90) {
                grid[i][j] = CellType::DESTRUCTIBLE_WALL;
            }
            else {
                grid[i][j] = CellType::INDESTRUCTIBLE_WALL;
            }
        }
    }
}

// Displays the map in the console for verification
void Map::displayMap() const {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            switch (cell) {
            case CellType::EMPTY: std::cout << "."; break;
            case CellType::DESTRUCTIBLE_WALL: std::cout << "D"; break;
            case CellType::INDESTRUCTIBLE_WALL: std::cout << "I"; break;
            }
        }
        std::cout << "\n";
    }
}

// Destroys a destructible wall at coordinates (x, y) if it exists
void Map::destroyWall(int x, int y) {

    if (x >= 0 && x < width && y >= 0 && y < height) {
        if (grid[x][y] == CellType::DESTRUCTIBLE_WALL) 
            grid[x][y] = CellType::EMPTY;  
        
    }
}

// Checks if there is a collision with a wall at coordinates (x, y)
bool Map::isCollisionWithWall(int x, int y) const {

    if (x >= 0 && x < width && y >= 0 && y < height) 
        return grid[x][y] == CellType::DESTRUCTIBLE_WALL || grid[x][y] == CellType::INDESTRUCTIBLE_WALL;
    
    return false;  
}
