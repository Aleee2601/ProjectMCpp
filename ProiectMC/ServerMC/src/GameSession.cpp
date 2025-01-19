#include "../include/GameSession.h"
#include <iostream>
#include <ranges>
#include <stdexcept> 
#include<unordered_map>
#include <chrono>
#include "../include/Utility.h"

GameSession::GameSession(int n, int m)
    : m_gameMap(std::make_shared<Map>(n, m)), m_currentTurn(0), m_gameOver(false), m_isFriendlyMode(false) {
}

GameSession::GameSession(std::shared_ptr<Map> map)
    : m_gameMap(map), m_currentTurn(0), m_gameOver(false), m_isFriendlyMode(false) {
}

void GameSession::StartGame() {
    m_currentTurn = 0;
    m_gameOver = false;

    int numPlayers = m_players.size();
    int mapWidth = m_gameMap->GetWidth(); 
    int mapHeight = m_gameMap->GetHeight(); 

    std::vector<std::pair<int, int>> startPositions = {
        {1, 1},
        {1, mapWidth - 2},
        {mapHeight - 2, 1},
        {mapHeight - 2, mapWidth - 2}
    };

    for (int i = 0; i < numPlayers; ++i) {
        auto& player = m_players[i];

        if (i < startPositions.size()) {
            auto& selectedPosition = startPositions[i];
            player.SetStartPosition(selectedPosition.first, selectedPosition.second);
        }
        else {
            std::cerr << "Not enough start positions for all players.\n";
        }
        player.SetStatus(PlayerStatus::ACTIVE); 
    }

    std::cout << "Game started with " << numPlayers << " players!\n";
    m_gameMap->DisplayMap(m_players,m_bullets);
}


void GameSession::AddPlayer(Player& player) {
    for (const auto& existingPlayer : m_players) {
        if (existingPlayer.GetId() == player.GetId()) {
            throw std::runtime_error("Player with ID " + std::to_string(player.GetId()) + " already exists.");
        }
    }
    m_players.push_back(player);
    std::cout << "Player " << m_players.back().GetName() << " has been added to the session.\n";
}

int GameSession::getCurrentTimeInSeconds() {
    return static_cast<int>(std::chrono::system_clock::now().time_since_epoch().count());
}

void GameSession::DisplayGameState() const {
    std::cout << "Current Game State:\n";
    for (const auto& player : m_players) {
        int x, y;
        player.GetPosition(x, y);
        std::cout << "Player " << player.GetName()
            << " is at position (" << x << ", " << y << ")\n";
    }
}

void GameSession::NextTurn() {
    do {
        m_currentTurn = (m_currentTurn + 1) % m_players.size();
    } while (IsCurrentPlayerEliminated()); 
}

Player& GameSession::GetCurrentPlayer() {
    return m_players[m_currentTurn];
}

bool GameSession::IsCurrentPlayerEliminated() {
    return GetCurrentPlayer().GetStatus() == PlayerStatus::ELIMINATED;
}

bool GameSession::UpdatePlayerPosition(int playerId, int newX, int newY) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            int currentX, currentY;
            player.GetPosition(currentX, currentY);

            for (const auto& otherPlayer : m_players) {
                if (otherPlayer.GetId() != playerId) {
                    int otherX, otherY;
                    otherPlayer.GetPosition(otherX, otherY);
                    if (otherX == newX && otherY == newY) {
                        std::cout << "Move blocked: Another player is already at position ("
                            << newX << ", " << newY << ").\n";
                        return false;
                    }
                }
            }

            if (m_gameMap->IsWithinBounds(newX, newY) &&
                !m_gameMap->IsCollisionWithWall(newX, newY)) {
                player.SetPosition(newX, newY); 
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

std::vector<Player>& GameSession::GetAllPlayers() {
    return m_players;
}

void GameSession::RemovePlayerById(int playerId) {
    auto initialSize = m_players.size(); 
    m_players.erase(
        std::remove_if(m_players.begin(), m_players.end(),
            [playerId](const Player& player) {
                return player.GetId() == playerId; 
            }),
        m_players.end()
    );
}


void GameSession::EndTurn() {
    std::cout << "Turn " << m_currentTurn + 1 << " ended.\n";

    if (CheckGameOver()) {
        EndGame();
    }
    else {
        NextTurn();
    }
}

bool GameSession::CheckGameOver() const {
    int activePlayers = std::ranges::count_if(m_players, [](const Player& player) {
        return player.GetStatus() == PlayerStatus::ACTIVE;
        });

    return activePlayers <= 1;
}

void GameSession::ResetSession() {
    m_players.clear();
   
    m_currentTurn = 0;
    m_gameMap = std::make_shared<Map>(m_gameMap->GetWidth(), m_gameMap->GetHeight());
    std::cout << "Game session has been reset.\n";
}

void GameSession::DisplayLeaderboard() const {
    auto sortedPlayers = m_players;

    std::ranges::sort(sortedPlayers, [](const Player& a, const Player& b) {
        return a.GetScore() > b.GetScore();
        });

    std::cout << "Leaderboard:\n";
    for (size_t i = 0; i < sortedPlayers.size(); ++i) {
        const auto& player = sortedPlayers[i];
        std::cout << i + 1 << ". " << player.GetName() << ": " << player.GetScore() << " points\n";
    }
}

Player* GameSession::GetPlayerById(int playerId) {
    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            return &player; 
        }
    }
    return nullptr; 
}

int GameSession::GetPlayerScore(int playerId) const {
    for (const auto& player : m_players) {
        if (player.GetId() == playerId) {
            return player.GetScore(); 
        }
    }
    throw std::runtime_error("Player not found");
}

bool GameSession::MovePlayer(int playerId, Direction direction) {
    std::pair<int, int> newPosition;

    for (auto& player : m_players) {
        if (player.GetId() == playerId) {
            int currentX, currentY;
            player.GetPosition(currentX, currentY); 

            newPosition = GetNextPosition(currentX, currentY, direction);

            return UpdatePlayerPosition(playerId, newPosition.first, newPosition.second);
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
        winner->AddWinScore(2); 
        winner->AwardWinnerBonus(); 
        std::cout << "Winner: " << winner->GetName() << " with score: " << winner->GetScore() << " points.\n";
    }

    auto runnerUp = std::max_element(m_players.begin(), m_players.end(),
        [&winner](const Player& a, const Player& b) {
            if (&a == &*winner) return true; 
            if (&b == &*winner) return false;
            return a.GetScore() < b.GetScore();
        });

    if (runnerUp != m_players.end() && runnerUp != winner) {
        runnerUp->AddWinScore(1); 
        std::cout << "Runner-up: " << runnerUp->GetName() << " with score: " << runnerUp->GetScore() << " points.\n";
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

void GameSession::StartFriendlyGame() {
    m_isFriendlyMode = true; 
    ResetForFriendlyMode();  
    AssignTeams();           
    StartGame();             
}
void GameSession::ResetForFriendlyMode() {
    for (auto& player : m_players) {
        player.ResetForFriendlyMode();
    }
}
void GameSession::AssignTeams() {
    if (!m_isFriendlyMode) return; 

    FriendlyModeData data;

    std::shuffle(m_players.begin(), m_players.end(), std::mt19937(std::random_device()()));

    std::vector<std::shared_ptr<Player>> galatiPlayers;
    std::vector<std::shared_ptr<Player>> brailaPlayers;

    for (size_t i = 0; i < m_players.size(); ++i) {
        auto playerPtr = std::make_shared<Player>(m_players[i]);
        if (i % 2 == 0) {
            data.galatiPlayers.push_back(playerPtr);
        }
        else {
            data.brailaPlayers.push_back(playerPtr);
        }
    }
    m_friendlyModeData = data;
}
bool GameSession::CanStartGame() const {

    if (m_players.size() < 2) { 
        std::cerr << "Not enough players to start the game.\n";
        return false;
    }

    if (m_gameOver) {
        std::cerr << "Game cannot start because it is already over.\n";
        return false;
    }

    return true; 
}

int GameSession::GetLobbyTimeRemaining() const {
    int lobbyStartTime = getCurrentTimeInSeconds();
    int currentTime = getCurrentTimeInSeconds();
    return std::max(30 - (currentTime - lobbyStartTime), 0); 
}

std::vector<Bullet> GameSession::GetAllBullets() const {
    std::vector<Bullet> allBullets;  

    for (const auto& player : m_players) {
        const auto& playerBullets = player.GetBulletsForPlayer();  
        allBullets.insert(allBullets.end(), playerBullets.begin(), playerBullets.end());  
    }

    return allBullets; 
}

void GameSession::MoveBullets(float deltaTime) {
 
    for (auto& player : m_players) {
        auto& bullets = player.GetWeapon().GetBullets(); 

        for (auto& bullet : bullets) {
            if (bullet.IsInactive()) continue;

            int nextX = bullet.GetX();
            int nextY = bullet.GetY();

            switch (bullet.GetDirection()) {
            case Direction::UP:    nextX--; break;
            case Direction::DOWN:  nextX++; break;
            case Direction::LEFT:  nextY--; break;
            case Direction::RIGHT: nextY++; break;
            }

            if (m_gameMap->IsCollisionWithWall(nextX, nextY)) {
                CellType cellType = m_gameMap->GetCellType(nextX, nextY);
                if (cellType == CellType::DESTRUCTIBLE_WALL) {
                    m_gameMap->DestroyWallWithDisplay(nextX, nextY,m_players,m_bullets);
                    m_gameMap->ActivateBombIfNeeded(nextX, nextY, m_players);
                    std::cout << "Bullet destroyed a destructible wall at (" << nextX << ", " << nextY << ").\n";
                }
                else if (cellType == CellType::INDESTRUCTIBLE_WALL) {
                    std::cout << "Bullet hit an indestructible wall at (" << nextX << ", " << nextY << ").\n";
                }
                bullet.SetInactive();
                continue;
            }

            for (auto& targetPlayer : m_players) {
                if (!targetPlayer.IsEliminated() && targetPlayer.GetX() == nextX && targetPlayer.GetY() == nextY) {
                    bullet.SetInactive();
                    targetPlayer.TakeHit();
                    std::cout << "Player " << targetPlayer.GetName() << " was hit by a bullet!\n";

                    if (targetPlayer.IsEliminated()) {
                        targetPlayer.SetStatus(PlayerStatus::ELIMINATED);
                        std::cout << "Player " << targetPlayer.GetName() << " has been eliminated!\n";
                        break;
                    }
                    break;
                }
            }

            if (!bullet.IsInactive()) {
                bullet.SetPosition(nextX, nextY);
            }
        }

        auto& bulletsVector = player.GetWeapon().GetBullets();
        bulletsVector.erase(
            std::remove_if(bulletsVector.begin(), bulletsVector.end(),
                [](const Bullet& b) { return b.IsInactive(); }),
            bulletsVector.end());
    }
}
