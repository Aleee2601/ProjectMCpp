﻿#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Map.h"
#include "Player.h"
#include <stdexcept> 

class GameSession {

public:
    // Constructor: Initializes the game session with a map of size n x m.
    GameSession(int n, int m); // Constructor cu dimensiuni
    GameSession(std::shared_ptr<Map> map); // Constructor cu hartă externă



    // Default constructor: Initializes the session with a default map size of 10 x 10.
    GameSession() : m_gameMap(10, 10), m_currentTurn(0) {}

    void StartGame();

    // Adds a player to the game session.
    void AddPlayer(Player&& player);

    // Displays the current state of the game.
    void DisplayGameState() const;

    // Simulates one player hitting another.
    void PlayerHits(Player& shooter, Player& target);

    // Moves to the next turn.
    void NextTurn();

    // Returns the player whose turn it is currently.
    Player& GetCurrentPlayer();

    // Checks if the current player has been eliminated.
    bool IsCurrentPlayerEliminated();

    // Starts the current turn.
    void StartTurn();

    // Updates the position of a specific player by their ID.
    bool UpdatePlayerPosition(int playerId, int newX, int newY);

    // Returns a list of all connected players.
    std::vector<Player> GetAllPlayers() const;

    // Removes a player from the session by their ID.
    void RemovePlayerById(int playerId);

    void EndTurn();

    bool CheckGameOver() const;

    // Records an event in the game session
    // Processes all recorded events.
    void ProcessEvents() const;

    // Resets the game session to its initial state.
    void ResetSession();

    // Displays the leaderboard of the game session.
    void DisplayLeaderboard() const;

    // Returns a reference to a player by their ID.
    Player& GetPlayerById(int playerId);

    // New methods:
    int GetPlayerScore(int playerId) const; // Returns the score of a player by their ID
    bool MovePlayer(int playerId, const std::string& direction); // Moves a player in a specified direction
    std::pair<int, int> GetPlayerPosition(int playerId) const; // Returns the position of a player by their ID

    void EndGame();
    //std::vector<Player> GetAllPlayers() const;
   


private:
    Map m_gameMap;                             // Game map
    std::vector<Player> m_players;             // List of players in the session
    int m_currentTurn;                         // Index of the current player's turn
    bool m_gameOver;
    std::unique_ptr<GameSession> m_gameSession;
};
