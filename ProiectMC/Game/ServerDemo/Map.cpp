#include "Map.h"
#include <algorithm>
#include <iostream>
#include <cmath>

Map::Map(int inaltime, int latime)
    : m_inaltime(inaltime),
    m_latime(latime),
    m_celule(inaltime, std::vector<CellType>(latime, CellType::FREE))
{
    initializeazaRandomEngine();
    genereazaHartaRandom();
}

// Move constructor
Map::Map(Map&& other) noexcept
{
    m_inaltime = other.m_inaltime;
    m_latime = other.m_latime;
    m_celule = std::move(other.m_celule);
    m_bombe = std::move(other.m_bombe);
    m_gen = std::move(other.m_gen);

    other.m_inaltime = 0;
    other.m_latime = 0;
    other.m_celule.clear();
    other.m_bombe.clear();
}

// Move assignment operator
Map& Map::operator=(Map&& other) noexcept
{
    if (this != &other)
    {
        m_inaltime = other.m_inaltime;
        m_latime = other.m_latime;
        m_celule = std::move(other.m_celule);
        m_bombe = std::move(other.m_bombe);
        m_gen = std::move(other.m_gen);

        other.m_inaltime = 0;
        other.m_latime = 0;
        other.m_celule.clear();
        other.m_bombe.clear();
    }
    return *this;
}

// Inițializare generator random
void Map::initializeazaRandomEngine()
{
    static std::random_device rd;
    m_gen.seed(rd());
}

// Generare random a hărții
void Map::genereazaHartaRandom()
{
    if (m_inaltime <= 0 || m_latime <= 0)
        return;

    std::uniform_real_distribution<double> dist(0.0, 1.0);

    m_bombe.clear();

    // Generăm structura hărții
    for (int i = 0; i < m_inaltime; ++i)
    {
        for (int j = 0; j < m_latime; ++j)
        {
            if (i == 0 || i == m_inaltime - 1 || j == 0 || j == m_latime - 1)
            {
                m_celule[i][j] = CellType::UNBREAKABLE;
            }
            else if ((j >= 10 && j <= 16) && (i >= 1 && i <= 4 || i >= 7 && i <= 10))
            {
                m_celule[i][j] = CellType::UNBREAKABLE;
            }
            else
            {
                m_celule[i][j] = CellType::FREE;
            }
        }
    }

    // Adăugăm până la 3 bombe random
    int nrBombe = std::uniform_int_distribution<int>(0, 3)(m_gen);
    for (int k = 0; k < nrBombe; ++k)
    {
        int bx = std::uniform_int_distribution<int>(1, m_inaltime - 2)(m_gen);
        int by = std::uniform_int_distribution<int>(1, m_latime - 2)(m_gen);

        if (m_celule[bx][by] == CellType::FREE)
        {
            m_bombe.emplace_back(Bomb{ bx, by, false });
        }
    }
}

// Verificare validitate coordonate
bool Map::pozitieValida(int x, int y) const
{
    return (x >= 0 && x < m_inaltime && y >= 0 && y < m_latime);
}

// Returnează tipul celulei
CellType Map::getCellType(int x, int y) const
{
    if (!pozitieValida(x, y))
    {
        throw std::out_of_range("Coordonate in afara hartii!");
    }
    return m_celule[x][y];
}

// Sparge zid
bool Map::breakWall(int x, int y)
{
    if (!pozitieValida(x, y)) return false;
    if (m_celule[x][y] == CellType::BREAKABLE)
    {
        // Spargem zidul
        m_celule[x][y] = CellType::FREE;

        // Verificăm dacă e o bombă
        if (existaBombaLa(x, y))
        {
            declanseazaBomba(x, y);
        }
        return true;
    }
    return false;
}

// Returnează înălțimea
int Map::getHeight() const
{
    return m_inaltime;
}

// Returnează lățimea
int Map::getWidth() const
{
    return m_latime;
}

// Verifică dacă există bombă la (x,y)
bool Map::existaBombaLa(int x, int y) const
{
    for (const auto& b : m_bombe)
    {
        if (b.x == x && b.y == y && !b.declansata)
        {
            return true;
        }
    }
    return false;
}

// Declanșează bomba
void Map::declanseazaBomba(int bx, int by)
{
    // Marcăm bomba ca declanșată
    for (auto& b : m_bombe)
    {
        if (b.x == bx && b.y == by && !b.declansata)
        {
            b.declansata = true;
            break;
        }
    }

    // Distrugem celulele breakable pe raza de 10
    constexpr double RAZA = 10.0;
    for (int i = 0; i < m_inaltime; ++i)
    {
        for (int j = 0; j < m_latime; ++j)
        {
            double distEuclid = std::sqrt((double)(i - bx) * (i - bx)
                + (double)(j - by) * (j - by));
            if (distEuclid <= RAZA)
            {
                if (m_celule[i][j] == CellType::BREAKABLE)
                {
                    m_celule[i][j] = CellType::FREE;
                }
            }
        }
    }
}
