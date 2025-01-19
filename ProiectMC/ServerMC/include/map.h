#pragma once
#include "../../Bomb/Bomb.h"
#include "Player.h"
#include <vector>
#include <iostream>
#include <random>

enum class CellType {
    EMPTY,
    DESTRUCTIBLE_WALL, 
    BOMB,
    INDESTRUCTIBLE_WALL
};

class Map {
private:
    std::vector<std::vector<CellType>> m_grid;
    int m_width;                               
    int m_height;                             
    std::vector<Bomb> m_bombs;    
    std::vector<int> m_bombsRadii;

public:

    Map(int n, int m);    
    Map();               

    CellType GetCellType(int x, int y) const;

    int GetWidth() const;

    int GetHeight() const;

    std::vector<Bomb> GetBombs() const;
    std::vector<std::pair<int, int>> GetBombPositions() const;
    std::vector<int> GetBombRadii() const;

    void SetCellType(int x, int y, CellType type);
    CellType getCellType(int x, int y) const;

    void GenerateRandomMap();

    void DestroyWall(int x, int y);

    void DestroyWallWithDisplay(int x, int y, std::vector<Player>& players,std::vector<Bullet>& bullets);

    void DisplayMap(const std::vector<Player>& players,const std::vector<Bullet>& bullets);

    void HandleBombEffect(int bombX, int bombY, std::vector<Player>& players);

    void ActivateBombIfNeeded(int x, int y, std::vector<Player>& players);

    std::vector<std::pair<int, int>> GetEmptyCells() const;

    bool IsWithinBounds(int x, int y) const;

    void CheckBulletCollisions(std::vector<Player>& players, std::vector<Bullet>& bullets);

    bool IsCollisionWithWall(int x, int y) const;

    void RemoveInactiveBombs();
    std::vector<Bomb>& GetAllBombs();
    const std::vector<Bomb>& GetAllBombs() const;
};