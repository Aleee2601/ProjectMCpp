#include "Map.h"
#include <random>

// Constructor that initializes the map with empty spaces and random configuration
Map::Map(int n, int m)
    : m_width(n), m_height(m), m_grid(n, std::vector<CellType>(m, CellType::EMPTY)) {
    GenerateRandomMap();
}

// Returns the type of cell at coordinates (x, y)
CellType Map::GetCellType(int x, int y) const {
    return m_grid.at(x).at(y);
}

// Returns the map's width
int Map::GetWidth() const {
    return m_width;
}

// Returns the map's height
int Map::GetHeight() const {
    return m_height;
}

// Sets the type of cell at coordinates (x, y), ensuring coordinates are within bounds
void Map::SetCellType(int x, int y, CellType type) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_grid[x][y] = type;
    }
}

// Generates a random map with destructible and indestructible walls
void Map::GenerateRandomMap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            int randValue = dist(gen);
            if (randValue < 70) {
                m_grid[i][j] = CellType::EMPTY;
            }
            else if (randValue < 90) {
                m_grid[i][j] = CellType::DESTRUCTIBLE_WALL;
            }
            else {
                m_grid[i][j] = CellType::INDESTRUCTIBLE_WALL;
            }
        }
    }
}

// Displays the map in the console for verification
void Map::DisplayMap() const {
    for (const auto& row : m_grid) {
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
void Map::DestroyWall(int x, int y) {
    if (IsWithinBounds(x, y) && m_grid[x][y] == CellType::DESTRUCTIBLE_WALL) {
        m_grid[x][y] = CellType::EMPTY;
    }
}

// Checks if there is a collision with a wall at coordinates (x, y)
bool Map::IsCollisionWithWall(int x, int y) const {
    return IsWithinBounds(x, y) &&
        (m_grid[x][y] == CellType::DESTRUCTIBLE_WALL || m_grid[x][y] == CellType::INDESTRUCTIBLE_WALL);
}

// Displays the map before and after wall destruction
void Map::DestroyWallWithDisplay(int x, int y) {
    std::cout << "Map before wall destruction:\n";
    DisplayMap();

    if (IsWithinBounds(x, y) && m_grid[x][y] == CellType::DESTRUCTIBLE_WALL) {
        DestroyWall(x, y);
        ActivateBombIfNeeded(x, y);
    }

    std::cout << "\nMap after wall destruction:\n";
    DisplayMap();
}

// Activates a bomb if one exists at the specified coordinates
void Map::ActivateBombIfNeeded(int x, int y) {
    if (IsWithinBounds(x, y) && m_grid[x][y] == CellType::DESTRUCTIBLE_WALL) {
        Bomb bomb(x, y);
        m_bombs.push_back(bomb);
        bomb.Detonate(*this);
    }
}

// Checks if the specified coordinates are within the bounds of the map
bool Map::IsWithinBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}
