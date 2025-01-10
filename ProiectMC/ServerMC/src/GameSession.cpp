#include "GameSession.h"
#include "Event.h"
#include "Player.h"
#include "map.h"
#include <iostream>
#include <stdexcept> 

// Adds a player to the game session
void GameSession::AddPlayer(const Player& player) {
    // Check if the player already exists in the session (by ID)
    for (const auto& existingPlayer : m_players) {
        if (existingPlayer.GetId() == player.GetId()) {
            std::cout << "Player " << player.GetName()
                << " with ID " << player.GetId()
                << " is already in the session.\n";
            return;
        }
    }

    // Add the player to the session
    m_players.push_back(player);
    std::cout << "Player " << player.GetName()
        << " has been added to the session.\n";
}

// Displays the current state of the game
void GameSession::DisplayGameState() const {
    std::cout << "Current Game State:\n";
    for (const auto& player : m_players) {
        int x, y;
        player.GetPosition(x, y);
        std::cout << "Player " << player.GetName()
            << " is at position (" << x << ", " << y << ")\n";
    }
}

// Simulates one player hitting another
void GameSession::PlayerHits(Player& shooter, Player& target) {
    target.TakeHit();
    shooter.AddScoreForHit();
}

// Moves to the next turn
void GameSession::NextTurn() {
    do {
        m_currentTurn = (m_currentTurn + 1) % m_players.size();
    } while (IsCurrentPlayerEliminated()); // Skip eliminated players
}

// Returns the player whose turn it is currently
Player& GameSession::GetCurrentPlayer() {
    return m_players[m_currentTurn];
}

// Checks if the current player has been eliminated
bool GameSession::IsCurrentPlayerEliminated() {
    return GetCurrentPlayer().GetStatus() == PlayerStatus::ELIMINATED;
}

// Starts the current turn
void GameSession::StartTurn() {
    std::cout << "\n--- Starting a new turn ---\n";
    std::cout << "It is now " << GetCurrentPlayer().GetName() << "'s turn\n";
}

// Updates the position of a specific player by their ID
bool GameSession::UpdatePlayerPosition(int playerId, int newX, int newY) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            player.SetPosition(newX, newY);
            return true;
        }
    }
    return false;
}

// Returns a list of all connected players
std::vector<Player> GameSession::GetAllPlayers() const {
    return m_players;
}

// Removes a player from the session by their ID
void GameSession::RemovePlayerById(int playerId) {
    for (auto it = m_players.begin(); it != m_players.end(); ++it) {
        if (it->GetId() == playerId) {
            std::cout << "Player " << it->GetName() << " with ID " << playerId << " has been removed from the session.\n";
            m_players.erase(it);
            return;
        }
    }
    std::cout << "Player with ID " << playerId << " not found.\n";
}

// Records an event in the game session
void GameSession::RecordEvent(std::unique_ptr<Event> event) {
    m_events.push_back(std::move(event));
}

// Processes all recorded events
void GameSession::ProcessEvents() const {
    for (const auto& event : m_events) {
        std::cout << event->getDescription() << std::endl;
    }
}

// Resets the game session to its initial state
void GameSession::ResetSession() {
    m_players.clear();
    m_events.clear();
    m_currentTurn = 0;
    m_gameMap = Map(m_gameMap.GetWidth(), m_gameMap.GetHeight());
    std::cout << "Game session has been reset.\n";
}

// Displays the leaderboard of the game session
void GameSession::DisplayLeaderboard() const {
    std::vector<Player> sortedPlayers = m_players;

    // Sort players by score in descending order
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& a, const Player& b) {
        return a.GetScore() > b.GetScore();
        });

    // Display the leaderboard
    std::cout << "Leaderboard:" << std::endl;
    for (size_t i = 0; i < sortedPlayers.size(); ++i) {
        const auto& player = sortedPlayers[i];
        std::cout << i + 1 << ". " << player.GetName() << ": " << player.GetScore() << " points" << std::endl;
    }
}

Player& GameSession::GetPlayerById(int playerId) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            return player;
        }
    }
    throw std::runtime_error("Player with ID " + std::to_string(playerId) + " not found");
}
