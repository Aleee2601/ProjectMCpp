#pragma once
#include "Bomb.h"
#include <vector>
#include <iostream>
#include <random>

enum class CellType { EMPTY, DESTRUCTIBLE_WALL, INDESTRUCTIBLE_WALL };

class Map {
private:
    std::vector<std::vector<CellType>> grid;  // Matrice pentru harta
    int width, height;
    std::vector<Bomb> bombs;
public:
    // Constructor care seteaza dimensiunile si initializeaza harta cu spatii libere
    Map(int n, int m);

    // Returneaza tipul de casuta de la coordonatele (x, y)
    CellType getCellType(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    // Seteaza tipul de casuta la coordonatele (x, y)
    void setCellType(int x, int y, CellType type);

    // Genereaza o harta cu pereti destructibili si indestructibili
    void generateRandomMap();

    // Afiseaza harta în consola pentru verificare
    void displayMap() const;

    // Destroys a destructible wall at coordinates (x, y)
    void destroyWall(int x, int y);

    // Checks if there is a collision with a wall at coordinates (x, y)
    bool isCollisionWithWall(int x, int y) const;

    // Destroys a wall and displays the map before and after destruction
    void destroyWallWithDisplay(int x, int y);

    void activateBombIfNeeded(int x, int y);

    bool isWithinBounds(int x, int y) const;
};