#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include "Player.h"

/**
 * @brief Clasa care conține logica jocului:
 *  - baza de date cu useri (login/register),
 *  - harta,
 *  - pozițiile jucătorilor,
 *  - funcții de startGame, move, shoot, etc.
 */
class GameLogic
{
public:
    enum class CellType
    {
        FREE,
        BREAKABLE,
        UNBREAKABLE
    };

    struct PlayerInfo
    {
        std::string username;
        int x;
        int y;
        // (opțional) direcție, viață, scor etc.
    };

public:
    GameLogic();
    ~GameLogic();

    // --------------------
    //   Funcții user
    // --------------------
    bool doRegister(const std::string& username, const std::string& password);
    bool doLogin(const std::string& username, const std::string& password);

    // --------------------
    //   Funcții joc
    // --------------------
    bool doStartGame();
    bool movePlayer(const std::string& username, int dx, int dy);
    void shoot(const std::string& username);
    void updateGameState();

    // --------------------
    //   Gettere
    // --------------------
    int getMapWidth() const { return m_mapWidth; }
    int getMapHeight() const { return m_mapHeight; }

    // Returnează celula (FREE/BREAKABLE/UNBREAKABLE)
    CellType getCell(int row, int col) const;

    // Returnează poziția curentă a jucătorului (sau -1,-1 dacă nu există)
    bool getPlayerPosition(const std::string& username, int& outX, int& outY) const;

    // Returnează toți jucătorii (pentru a-i serializa ulterior)
    std::vector<Player> getAllPlayers() const;


private:
    void initializeMap();

private:
    // Bază de date locală: user -> parola
    std::unordered_map<std::string, std::string> m_userDatabase;

    // Dicționar jucători: user -> PlayerInfo
    std::unordered_map<std::string, Player> m_players;          

    // Harta
    std::vector<std::vector<CellType>> m_map;
    int m_mapWidth;
    int m_mapHeight;

    // Mutex pentru sincronizare (dacă avem mai multe thread-uri)
    mutable std::mutex m_mutex;
};