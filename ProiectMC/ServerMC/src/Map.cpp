#include "../include/Map.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>


Map::Map(int n, int m)
    : m_width(n), m_height(m), m_grid(n, std::vector<CellType>(m, CellType::EMPTY)), m_bombs() {
    if (n <= 0 || m <= 0) {
        throw std::invalid_argument("Map dimensions must be greater than 0.");
    }
    GenerateRandomMap(); 
}

Map::Map() : m_width(0), m_height(0), m_grid(), m_bombs() {}

CellType Map::GetCellType(int x, int y) const {
    if (IsWithinBounds(x, y)) {
        return m_grid[x][y];
    }
    return CellType::EMPTY; 
}

int Map::GetWidth() const {
    return m_width;
}

std::vector<Bomb> Map::GetBombs() const {
    return m_bombs;
}

int Map::GetHeight() const {
    return m_height;
}
std::vector<std::pair<int, int>> Map::GetBombPositions() const {
    std::vector<std::pair<int, int>> bombPositions;
    for (const auto& bomb : m_bombs) {
        bombPositions.emplace_back(bomb.GetPosition()); 
    }
    return bombPositions;
}

std::vector<int> Map::GetBombRadii() const {
    return m_bombsRadii;
}

void Map::SetCellType(int x, int y, CellType type) {
    if (IsWithinBounds(x, y)) {
        m_grid[x][y] = type;
    }
}
CellType Map::getCellType(int x, int y) const {
    if (x < 0 || x >= m_height || y < 0 || y >= m_width) {
        return CellType::EMPTY; 
    }
    return m_grid[x][y];
}

void Map::GenerateRandomMap() {
    m_grid = std::vector<std::vector<CellType>>(m_height, std::vector<CellType>(m_width, CellType::INDESTRUCTIBLE_WALL));

    for (int i = 1; i < m_height - 1; ++i) {
        for (int j = 1; j < m_width - 1; ++j) {

            if ((j >= 2 && j <= 10) || (j >= m_width - 11 && j <= m_width - 3)) {
                m_grid[i][j] = CellType::EMPTY;
            }
            if (i >= m_height / 2 - 4 && i <= m_height / 2 + 4) {
                if (j >= 2 && j <= m_width - 3) {
                    m_grid[i][j] = CellType::EMPTY;
                }
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99); 

    for (int i = 1; i < m_height - 1; ++i) {
        for (int j = 1; j < m_width - 1; ++j) {
            if ((i == 1 && (j == m_width - 2 || j == 1)) || (i == m_height - 2 && (j == m_width - 2 || j == 1)))
                continue;
            if (m_grid[i][j] == CellType::EMPTY) {
                int chance = dist(gen);
                if (chance < 20) { 
                    m_grid[i][j] = CellType::DESTRUCTIBLE_WALL;
                }
                else if (chance < 25) { 
                    m_grid[i][j] = CellType::INDESTRUCTIBLE_WALL;
                }
            }
        }
    }

    std::uniform_int_distribution<> bombDist(1, m_width - 2);
    const int numBombs = 3; 
    for (int i = 0; i < numBombs; ++i) {
        int x = bombDist(gen);
        int y = bombDist(gen);

        while (m_grid[y][x] != CellType::DESTRUCTIBLE_WALL) {
            x = bombDist(gen);
            y = bombDist(gen);
        }
        m_bombs.emplace_back(x, y); 
        m_bombsRadii.push_back(10); 
    }
    
}

void Map::DestroyWall(int x, int y) {
    if (IsWithinBounds(x, y) && m_grid[x][y] == CellType::DESTRUCTIBLE_WALL) {
        m_grid[x][y] = CellType::EMPTY;
    }
}
void Map::DestroyWallWithDisplay(int x, int y, std::vector<Player>& players, std::vector<Bullet>& bullets) {

    if (IsWithinBounds(x, y)) {
        DestroyWall(x, y);
        ActivateBombIfNeeded(x, y, players); 
    }

    std::cout << "\nMap after wall destruction:\n";
    DisplayMap(players,bullets);
}

void Map::DisplayMap(const std::vector<Player>& players,const std::vector<Bullet>& bullets) {

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            bool playerFound = false;
            bool bulletFound = false;

            for (const auto& player : players) {
                int playerX, playerY;
                player.GetPosition(playerX, playerY);

                if (playerX == i && playerY == j && !player.IsEliminated()) {
                    std::cout << "X";  
                    playerFound = true;
                    break;
                }
            }
            if (!playerFound) {
                for (const auto& bullet : bullets) {
                    if (bullet.GetX() == i && bullet.GetY() == j && !bullet.IsInactive()) {
                        std::cout << "*";  
                        bulletFound = true;
                        break;
                    }
                }
            }
       
            if (!playerFound && !bulletFound) {
                switch (m_grid[i][j]) {
                case CellType::EMPTY: std::cout << "."; break;
                case CellType::DESTRUCTIBLE_WALL: std::cout << "D"; break;
                case CellType::INDESTRUCTIBLE_WALL: std::cout << "I"; break;
                }
            }
        }
        std::cout << "\n"; 
    }
}


void Map::HandleBombEffect(int bombX, int bombY, std::vector<Player>& players) {
    const int radius = 10; 

    for (int x = bombX - radius; x <= bombX + radius; ++x) {
        for (int y = bombY - radius; y <= bombY + radius; ++y) {
            if (IsWithinBounds(x, y)) {

                if (m_grid[x][y] == CellType::DESTRUCTIBLE_WALL) {
                    m_grid[x][y] = CellType::EMPTY;
                }

                for (auto& player : players) {
                    if (player.IsEliminated()) continue;

                    if (player.GetX() == x && player.GetY() == y) {
                        player.SetStatus(PlayerStatus::ELIMINATED);
                        player.ResetPosition();
                        std::cout << "Player " << player.GetName() << " was eliminated by a bomb!\n";
                    }
                }
            }
        }
    }
}


void Map::ActivateBombIfNeeded(int x, int y, std::vector<Player>& players) {
    for (auto& bomb : m_bombs) {
        if (!bomb.IsActive() && bomb.GetPosition() == std::make_pair(x, y)) {
            bomb.Activate();
            std::cout << "Bomb activated at (" << x << ", " << y << ")!\n";

            HandleBombEffect(x, y, players);

            bomb.Deactivate();
            break;
        }
    }
}

bool Map::IsWithinBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}
void Map::CheckBulletCollisions(std::vector<Player>& players, std::vector<Bullet>& bullets) {
    for (auto& bullet : bullets) { 
        if (bullet.IsInactive()) continue;

        if (IsCollisionWithWall(bullet.GetX(), bullet.GetY())) {
            CellType cellType = GetCellType(bullet.GetX(), bullet.GetY());

            if (cellType == CellType::DESTRUCTIBLE_WALL) {
                DestroyWall(bullet.GetX(), bullet.GetY());

                ActivateBombIfNeeded(bullet.GetX(), bullet.GetY(), players);

                std::cout << "Bullet destroyed a destructible wall at (" << bullet.GetX() << ", " << bullet.GetY() << ").\n";
            }
            else if (cellType == CellType::INDESTRUCTIBLE_WALL) {
                std::cout << "Bullet hit an indestructible wall at (" << bullet.GetX() << ", " << bullet.GetY() << ").\n";
            }


            bullet.SetInactive();
            continue; 
        }


        for (auto& player : players) {
            if (player.IsEliminated()) continue;


            if (bullet.GetX() == player.GetX() && bullet.GetY() == player.GetY()) {
                bullet.SetInactive();  
                player.TakeHit();      

                std::cout << "Player " << player.GetName() << " was hit by a bullet!\n";

                if (player.IsEliminated()) {
                    player.SetStatus(PlayerStatus::ELIMINATED);
                    player.ResetPosition(); 

                    int shooterId = bullet.GetOwnerId();
                    auto shooter = std::find_if(players.begin(), players.end(),
                        [shooterId](const Player& p) { return p.GetId() == shooterId; });

                    if (shooter != players.end()) {
                        shooter->AddScoreForHit(); 
                        std::cout << "Player " << shooter->GetName() << " scored 100 points for hitting "
                            << player.GetName() << "!\n";
                    }

                    std::cout << "Player " << player.GetName() << " has been eliminated!\n";
                }
            }
            else
                for (size_t i = 0; i < bullets.size(); ++i) {
                    if (bullets[i].IsInactive()) continue;

                    for (size_t j = i + 1; j < bullets.size(); ++j) {
                        if (bullets[j].IsInactive()) continue;

                        if (bullets[i].GetX() == bullets[j].GetX() && bullets[i].GetY() == bullets[j].GetY()) {
                            bullets[i].SetInactive();
                            bullets[j].SetInactive();
                            std::cout << "Two bullets collided at (" << bullets[i].GetX() << ", " << bullets[i].GetY() << ") and were deactivated.\n";
                        }
                    }
                }
            
        }
    }
}

bool Map::IsCollisionWithWall(int x, int y) const {
    if (!IsWithinBounds(x, y)) {
        return true;
    }

    CellType cellType = GetCellType(x, y);
    if (cellType == CellType::DESTRUCTIBLE_WALL || cellType == CellType::INDESTRUCTIBLE_WALL) {
        return true;
    }

    return false;
}
std::vector<std::pair<int, int>> Map::GetEmptyCells() const {
    std::vector<std::pair<int, int>> emptyCells;

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_grid[i][j] == CellType::EMPTY) {
                emptyCells.push_back({ i, j });  
            }
        }
    }

    return emptyCells; 
}
void Map::RemoveInactiveBombs() {
    m_bombs.erase(
        std::remove_if(m_bombs.begin(), m_bombs.end(), [](const Bomb& bomb) {
            return !bomb.IsActive(); 
            }),
        m_bombs.end()  
    );

    std::cout << "Inactive bombs removed.\n";
}

std::vector<Bomb>& Map::GetAllBombs() {
    return m_bombs;
}
const std::vector<Bomb>& Map::GetAllBombs() const {
    return m_bombs;
}
