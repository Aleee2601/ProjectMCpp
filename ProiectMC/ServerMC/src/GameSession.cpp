#include "../include/GameSession.h"
#include <iostream>
#include <stdexcept> 
#include<unordered_map>


GameSession::GameSession(int n, int m) : m_gameMap(n, m), m_currentTurn(0), m_gameOver(false) {}

GameSession::GameSession(std::shared_ptr<Map> map) : m_gameMap(*map), m_currentTurn(0), m_gameOver(false) {}


void GameSession::StartGame() {
    m_currentTurn = 0;
    m_gameOver = false;

    int numPlayers = m_players.size();
    int mapWidth = m_gameMap.GetWidth();
    int mapHeight = m_gameMap.GetHeight();

    // Poziții de start
    std::vector<std::pair<int, int>> startPositions = {
        {0, 0},
        {0, mapWidth - 1},
        {mapHeight - 1, 0},
        {mapHeight - 1, mapWidth - 1}
    };

    for (int i = 0; i < numPlayers; ++i) {
        auto& player = m_players[i];
        int startX = startPositions[i].first;
        int startY = startPositions[i].second;

        player.SetPosition(startX, startY);
        player.ResetPosition(); // Asigură-te că poziția inițială este rescrisă
        player.SetStatus(PlayerStatus::ACTIVE);
    }

    std::cout << "Game started with " << numPlayers << " players!\n";
}


// Adds a player to the game session
//void GameSession::AddPlayer(const Player& player) {
//    // Check if the player already exists in the session (by ID)
//    for (const auto& existingPlayer : m_players) {
//        if (existingPlayer.GetId() == player.GetId()) {
//            std::cout << "Player " << player.GetName()
//                << " with ID " << player.GetId()
//                << " is already in the session.\n";
//            return;
//        }
//    }
//
//    // Add the player to the session
//    m_players.push_back(player);
//    std::cout << "Player " << player.GetName()
//        << " has been added to the session.\n";
//}

//void GameSession::AddPlayer(Player&& player) {
//    m_players.push_back(std::move(player)); // Mutăm player-ul în vector
//    std::cout << "Player " << m_players.back().GetName() << " has been added to the session.\n";
//}
void GameSession::AddPlayer(Player&& player) {
    for (const auto& existingPlayer : m_players) {
        if (existingPlayer.GetId() == player.GetId()) {
            throw std::runtime_error("Player with ID " + std::to_string(player.GetId()) + " already exists.");
        }
    }
    m_players.push_back(std::move(player));
    std::cout << "Player " << m_players.back().GetName() << " has been added to the session.\n";
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
    std::cout << "Turn " << m_currentTurn + 1 << " started. Current player: "
        << GetCurrentPlayer().GetName() << "\n";
}

// Updates the position of a specific player by their ID
bool GameSession::UpdatePlayerPosition(int playerId, int newX, int newY) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            int currentX, currentY;
            player.GetPosition(currentX, currentY); // Obținem poziția curentă

            // Verificăm dacă noua poziție este validă pe hartă
            if (m_gameMap.IsWithinBounds(newX, newY) &&
                !m_gameMap.IsCollisionWithWall(newX, newY)) {
                player.SetPosition(newX, newY); // Setăm poziția nouă
                std::cout << "Player " << player.GetName() << " moved from ("
                    << currentX << ", " << currentY << ") to ("
                    << newX << ", " << newY << ").\n";
                return true;
            }
            else {
                std::cout << "Invalid move for player " << player.GetName()
                    << " to position (" << newX << ", " << newY << ").\n";
            }
            return false;
        }
    }
    std::cout << "Player with ID " << playerId << " not found.\n";
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
void GameSession::EndTurn() {
    std::cout << "Turn " << m_currentTurn + 1 << " ended.\n";

    // Verifică condițiile de finalizare a jocului
    if (CheckGameOver()) {
        EndGame();
    }
    else {
        NextTurn();
    }
}

bool GameSession::CheckGameOver() const {
    int activePlayers = 0;
    for (const auto& player : m_players) {
        if (player.GetStatus() == PlayerStatus::ACTIVE) {
            ++activePlayers;
            if (activePlayers > 1) return false; // Mai sunt cel puțin 2 jucători activi
        }
    }
    return true; // Jocul s-a terminat
}


// Resets the game session to its initial state
void GameSession::ResetSession() {
    m_players.clear();
   
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

//Player& GameSession::GetPlayerById(int playerId) {
//    for (auto& player : m_players) {
//        if (player.GetId() == playerId) {
//            return player;
//        }
//    }
//    throw std::runtime_error("Player with ID " + std::to_string(playerId) + " not found");
//}

Player& GameSession::GetPlayerById(int playerId) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            return player;
        }
    }
    throw std::runtime_error("Player with ID " + std::to_string(playerId) + " not found.");
}



// ------------
int GameSession::GetPlayerScore(int playerId) const {
    for (const auto& player : m_players) {
        if (player.GetId() == playerId) {
            return player.GetScore(); 
        }
    }
    throw std::runtime_error("Player not found");
}

bool GameSession::MovePlayer(int playerId, const std::string& direction) {
    static const std::unordered_map<std::string, std::pair<int, int>> moves = {
        {"up", {0, -1}}, {"down", {0, 1}},
        {"left", {-1, 0}}, {"right", {1, 0}}
    };

    auto moveIt = moves.find(direction);
    if (moveIt == moves.end()) return false; // Direcție invalidă

    int dx = moveIt->second.first;
    int dy = moveIt->second.second;

    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            int currentX, currentY;
            player.GetPosition(currentX, currentY);
            return UpdatePlayerPosition(playerId, currentX + dx, currentY + dy);
        }
    }

    throw std::runtime_error("Player not found");
}


std::pair<int, int> GameSession::GetPlayerPosition(int playerId) const {
    for (const auto& player : m_players) {
        if (player.GetId() == playerId) {
            return { player.GetX(), player.GetY() };
        }
        throw std::runtime_error("Player not found");
    }
}

//void GameSession::EndGame() {
//    m_gameOver = true;
//    std::cout << "Game Over!\n";
//
//    if (m_players.empty()) {
//        std::cout << "No players in the game.\n";
//        return;
//    }
//
//    // Determină câștigătorul și afișează clasamentul
//    DisplayLeaderboard();
//    auto winner = std::max_element(m_players.begin(), m_players.end(),
//        [](const Player& a, const Player& b) { return a.GetScore() < b.GetScore(); });
//
//    if (winner != m_players.end()) {
//        std::cout << "Winner: " << winner->GetName() << " with score: " << winner->GetScore() << " points.\n";
//    }
//}
void GameSession::EndGame() {
    m_gameOver = true;
    std::cout << "Game Over!\n";

    if (m_players.empty()) {
        std::cout << "No players in the game.\n";
        return;
    }

    DisplayLeaderboard();

    auto winner = std::max_element(m_players.begin(), m_players.end(),
        [](const Player& a, const Player& b) { return a.GetScore() < b.GetScore(); });

    if (winner != m_players.end()) {
        std::cout << "Winner: " << winner->GetName() << " with score: " << winner->GetScore() << " points.\n";
    }

    int totalPoints = 0;
    int eliminatedPlayers = 0;
    for (const auto& player : m_players) {
        totalPoints += player.GetScore();
        if (player.GetStatus() == PlayerStatus::ELIMINATED) {
            eliminatedPlayers++;
        }
    }

    std::cout << "Total points scored: " << totalPoints << "\n";
    std::cout << "Players eliminated: " << eliminatedPlayers << "/" << m_players.size() << "\n";
}


