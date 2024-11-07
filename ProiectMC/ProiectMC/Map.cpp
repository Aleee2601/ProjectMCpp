module map;

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
// Generare aleatorie a hărții cu pereți destructibili și indestructibili
void Map::generateRandomMap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);  // Interval pentru randomizare

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int randValue = dist(gen);

            if (randValue < 70) {
                grid[i][j] = CellType::EMPTY;  // 70% șanse pentru spațiu liber
            }
            else if (randValue < 90) {
                grid[i][j] = CellType::DESTRUCTIBLE_WALL;  // 20% șanse pentru zid destructibil
            }
            else {
                grid[i][j] = CellType::INDESTRUCTIBLE_WALL;  // 10% șanse pentru zid indestructibil
            }
        }
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
