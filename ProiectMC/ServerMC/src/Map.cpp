#include "../include/Map.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


// Constructor: Initializes the map with empty spaces and random configuration
Map::Map(int n, int m)
    : m_width(n), m_height(m), m_grid(n, std::vector<CellType>(m, CellType::EMPTY)), m_bombs() {
    GenerateRandomMap(); // Generează harta aleatorie

}

//// Constructor implicit: Creează o hartă goală
//Map::Map() : m_width(0), m_height(0), m_grid(0, std::vector<CellType>(0, CellType::EMPTY)) {}

// Returns the type of cell at coordinates (x, y)
CellType Map::GetCellType(int x, int y) const {
    if (IsWithinBounds(x, y)) {
        return m_grid[x][y];
    }
    return CellType::EMPTY; // Returnează EMPTY pentru coordonate invalide
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
    if (IsWithinBounds(x, y)) {
        m_grid[x][y] = type;
    }
}

// Generates a random map with destructible and indestructible walls
void Map::GenerateRandomMap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99); // Pentru pereți și spații libere
    std::uniform_int_distribution<> bombDist(0, m_width - 1); // Pentru poziția bombelor

    int numBombs = 5; // Numărul de bombe dorit

    // Initialize the grid with empty cells
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            m_grid[i][j] = CellType::EMPTY;
        }
    }

    // Add random destructible and indestructible walls
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_grid[i][j] == CellType::EMPTY) { // Doar pe celule libere
                int chance = dist(gen);
                if (chance < 10) { // 10% șanse pentru pereți destructibili
                    m_grid[i][j] = CellType::DESTRUCTIBLE_WALL;
                }
                else if (chance < 15) { // 5% șanse pentru pereți indestructibili
                    m_grid[i][j] = CellType::INDESTRUCTIBLE_WALL;
                }
            }
        }
    }

    // Generate random bombs
    for (int i = 0; i < numBombs; ++i) {
        int x = bombDist(gen);
        int y = bombDist(gen);

        // Verificăm să plasăm bomba doar pe celule libere
        while (m_grid[x][y] != CellType::EMPTY) {
            x = bombDist(gen);
            y = bombDist(gen);
        }

        m_bombs.emplace_back(x, y); // Adaugă bomba
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

// Destroys a wall and displays the map before and after destruction
void Map::DestroyWallWithDisplay(int x, int y) {
    std::cout << "Map before wall destruction:\n";
    DisplayMap();

    if (IsWithinBounds(x, y)) {
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

// Removes bombs that are no longer active
void Map::RemoveInactiveBombs() {
    m_bombs.erase(
        std::remove_if(m_bombs.begin(), m_bombs.end(),
            [](const Bomb& bomb) -> bool {
                // Verificăm dacă bomba este inactivă
                return bomb.IsInactive(); // Funcție definită în clasa Bomb
            }),
        m_bombs.end());
}

//bool IsBombInactive(const Bomb& bomb) {
//    // Logica ta pentru a determina dacă bomba este inactivă
//    return false; // Exemplu: schimbă cu logica reală
//}
//
//
//void Map::RemoveInactiveBombs() {
//    m_bombs.erase(
//        std::remove_if(m_bombs.begin(), m_bombs.end(), IsBombInactive),
//        m_bombs.end());
//}


// Checks if the specified coordinates are within the bounds of the map
bool Map::IsWithinBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}