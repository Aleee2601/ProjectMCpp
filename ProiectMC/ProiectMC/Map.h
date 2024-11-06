#pragma once
#include <vector>
#include <iostream>
#include <random>

enum class CellType { EMPTY, DESTRUCTIBLE_WALL, INDESTRUCTIBLE_WALL };

class Map {
private:
    std::vector<std::vector<CellType>> grid;  // Matrice pentru harta
    int width, height;

public:
    // Constructor care setează dimensiunile și initializează harta cu spații libere
    Map(int n, int m);

    // Returnează tipul de căsuță de la coordonatele (x, y)
    CellType getCellType(int x, int y) const;

    // Setează tipul de căsuță la coordonatele (x, y)
    void setCellType(int x, int y, CellType type);


    // Afișează harta în consolă pentru verificare
    void displayMap() const;
};
