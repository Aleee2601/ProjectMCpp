#pragma once

#include <vector>
#include <random>
#include <stdexcept>

// Tipurile de celule din hartă
enum class CellType
{
    FREE,
    BREAKABLE,
    UNBREAKABLE
};

// Structură pentru bombă
struct Bomb
{
    int x;
    int y;
    bool declansata = false;
};

class Map
{
public:
    // Constructor
    Map(int inaltime, int latime);

    // Move constructor
    Map(Map&& other) noexcept;

    // Move assignment operator
    Map& operator=(Map&& other) noexcept;

    // Generează random harta
    void genereazaHartaRandom();

    // Verifică dacă (x,y) e valid în hartă
    bool isValidPosition(int x, int y) const { return pozitieValida(x, y); }

    // Obține tipul celulei (aruncă excepție dacă e out of range)
    CellType getCellType(int x, int y) const;

    // Sparge zid la (x,y) dacă e BREAKABLE, întoarce true dacă s-a spart
    bool breakWall(int x, int y);

    // Returnează înălțimea și lățimea hărții
    int getHeight() const;
    int getWidth() const;

private:
    // Metodă internă pentru verificare
    bool pozitieValida(int x, int y) const;

    // Verifică dacă există bombă la (x,y)
    bool existaBombaLa(int x, int y) const;

    // Declanșează bomba de la (bx,by)
    void declanseazaBomba(int bx, int by);

    // Inițializare generator random
    void initializeazaRandomEngine();

private:
    int m_inaltime;
    int m_latime;
    std::vector<std::vector<CellType>> m_celule;
    std::vector<Bomb> m_bombe;

    // Generator random
    std::mt19937 m_gen;
};
