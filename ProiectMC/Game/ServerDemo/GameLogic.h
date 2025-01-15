#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <algorithm>
#include <chrono>
#include "Player.h" // Include clasa Player

/**
 * @brief Clasa care conține logica jocului:
 *  - baza de date cu utilizatori (login/register),
 *  - harta,
 *  - pozițiile jucătorilor,
 *  - funcții de startGame, move, shoot, etc.
 */

 // Structura pentru jucători în așteptare
struct WaitingPlayer {
    std::string username;
    int score;
    std::chrono::steady_clock::time_point joinTime;
};

// Structura pentru jocuri active
struct GameSession {
    std::vector<std::string> players; // Lista jucătorilor din acest joc
    int id; // ID-ul unic al jocului
};

class GameLogic {
public:
    enum class CellType {
        FREE,
        BREAKABLE,
        UNBREAKABLE
    };

    GameLogic();
    ~GameLogic();

    // Funcții pentru utilizatori
    bool doRegister(const std::string& username, const std::string& password);
    bool doLogin(const std::string& username, const std::string& password);

    // Funcții pentru joc
    void addPlayerToWaitingList(const std::string& username, int score);
    void distributePlayers();
    bool movePlayer(const std::string& username, int dx, int dy);
    void shoot(const std::string& username);


    // Funcții pentru hartă
    int getMapWidth() const { return m_mapWidth; }
    int getMapHeight() const { return m_mapHeight; }
    CellType getCell(int row, int col) const;

    // Gettere
    const std::vector<GameSession>& getActiveGames() const;
    Player& getPlayer(const std::string& username);
    std::vector<Player> getAllPlayers() const;
    const Player* getPlayerInfo(const std::string& username) const;
    std::vector<std::string> getLobbyPlayers(int gameId);


private:
    void initializeMap();

    // Membri privați
    std::unordered_map<std::string, std::string> m_userDatabase; // Username -> Password
    std::unordered_map<std::string, Player> m_players;           // Username -> Player
    std::vector<WaitingPlayer> m_waitingPlayers;                 // Jucători în așteptare
    std::vector<GameSession> m_activeGames;                      // Lista jocurilor active
    mutable std::mutex m_mutex;                                  // Mutex pentru sincronizare

    // Hartă
    std::vector<std::vector<CellType>> m_map;
    int m_mapWidth;
    int m_mapHeight;
};
