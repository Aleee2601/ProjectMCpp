#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include "Map.h"
#include <stdexcept> 
#include <chrono>
import direction;

struct FriendlyModeData {
    std::vector<std::shared_ptr<Player>> galatiPlayers; 
    std::vector<std::shared_ptr<Player>> brailaPlayers; 
    FriendlyModeData() = default; 
};

class GameSession {

public:
    GameSession(int n, int m);
    GameSession(std::shared_ptr<Map> map); 

    void StartGame();

    void AddPlayer(Player& player);

    void DisplayGameState() const;

    void NextTurn();

    Player& GetCurrentPlayer();

    bool IsCurrentPlayerEliminated();

    bool UpdatePlayerPosition(int playerId, int newX, int newY);

    std::vector<Player>& GetAllPlayers();

    void RemovePlayerById(int playerId);

    void EndTurn();

    bool CheckGameOver() const;

    void ProcessEvents() const;

    void ResetSession();

    void DisplayLeaderboard() const;

    Player* GetPlayerById(int playerId);

    int GetPlayerScore(int playerId) const; 
    bool MovePlayer(int playerId, Direction direction); 
    std::pair<int, int> GetPlayerPosition(int playerId) const; 

    void EndGame();
    void StartFriendlyGame();
    void ResetForFriendlyMode();
    void AssignTeams();
    static int getCurrentTimeInSeconds();
    std::vector<Bullet> GetAllBullets() const;
    void MoveBullets(float deltaTime);
    void StartLobby() { 
        m_lobbyStartTime = getCurrentTimeInSeconds();
    }
    bool CanStartGame() const;
    int GetLobbyTimeRemaining() const;

private:

    std::shared_ptr<Map> m_gameMap; 
    std::vector<Player> m_players;             
    std::vector<Bullet> m_bullets;             
    int m_currentTurn;                       
    bool m_gameOver;
    bool  m_isFriendlyMode;
    int m_lobbyStartTime;
    std::optional<FriendlyModeData> m_friendlyModeData; 
    std::unique_ptr<GameSession> m_gameSession;
};
