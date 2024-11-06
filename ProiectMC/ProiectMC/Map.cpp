#include "Map.h"

// Constructor care inițializează harta cu spații libere și generează o configurație aleatorie
Map::Map(int n, int m) : width(n), height(m), grid(n, std::vector<CellType>(m, CellType::EMPTY)) {
    generateRandomMap();
}

// Returnează tipul de căsuță la coordonatele (x, y)
CellType Map::getCellType(int x, int y) const {
    return grid.at(x).at(y);
}

// Setează tipul de căsuță la coordonatele (x, y)
void Map::setCellType(int x, int y, CellType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[x][y] = type;
    }
}


// Afișează harta în consolă pentru verificare
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
