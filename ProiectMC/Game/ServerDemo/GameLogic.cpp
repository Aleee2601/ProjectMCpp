#include "GameLogic.h"
#include <iostream>
#include <stdexcept>

GameLogic::GameLogic() : m_mapWidth(27), m_mapHeight(12) {
    initializeMap();
}

GameLogic::~GameLogic() {}

bool GameLogic::doRegister(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_userDatabase.count(username) > 0) {
        return false;
    }
    m_userDatabase[username] = password;
    return true;
}

bool GameLogic::doLogin(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_userDatabase.count(username) > 0 && m_userDatabase[username] == password) {
        if (m_players.find(username) == m_players.end()) {
            // Creează un nou Player
            m_players[username] = Player(username, 0, 0); // Poziția inițială
        }
        return true;
    }
    return false;
}

void GameLogic::addPlayerToWaitingList(const std::string& username, int score) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_waitingPlayers.push_back({ username, score, std::chrono::steady_clock::now() });
}

void GameLogic::distributePlayers() {
    std::lock_guard<std::mutex> lock(m_mutex);

    static int nextGameId = 1; // Contor global pentru ID-urile jocurilor

    // Sortează jucătorii în funcție de scor
    std::sort(m_waitingPlayers.begin(), m_waitingPlayers.end(),
        [](const WaitingPlayer& a, const WaitingPlayer& b) {
            return a.score > b.score; // Descrescător după scor
        });

    // Formează jocuri de câte 4 jucători
    while (m_waitingPlayers.size() >= 2) {
        std::vector<std::string> newGamePlayers;
        for (int i = 0; i < 4 && !m_waitingPlayers.empty(); ++i) {
            newGamePlayers.push_back(m_waitingPlayers.front().username);
            m_waitingPlayers.erase(m_waitingPlayers.begin());
        }
        m_activeGames.push_back({ newGamePlayers });
    }
}


bool GameLogic::movePlayer(const std::string& username, int dx, int dy) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_players.find(username);
    if (it == m_players.end()) {
        return false;
    }

    Player& player = it->second;
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    if (newX < 0 || newX >= m_mapWidth || newY < 0 || newY >= m_mapHeight || m_map[newY][newX] != CellType::FREE) {
        return false;
    }

    player.move(dx, dy);
    return true;
}

void GameLogic::shoot(const std::string& username) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_players.find(username);
    if (it != m_players.end()) {
        Player& player = it->second;
        if (player.canShoot()) {
            player.recordShot();
            std::cout << "[Server] " << username << " a tras un glonț!\n";
        }
        else {
            std::cout << "[Server] " << username << " nu poate trage încă!\n";
        }
    }
}

void GameLogic::initializeMap() {
    m_map = std::vector<std::vector<CellType>>(m_mapHeight, std::vector<CellType>(m_mapWidth, CellType::FREE));
    for (int row = 0; row < m_mapHeight; ++row) {
        for (int col = 0; col < m_mapWidth; ++col) {
            if (row == 0 || row == m_mapHeight - 1 || col == 0 || col == m_mapWidth - 1) {
                m_map[row][col] = CellType::UNBREAKABLE;
            }
        }
    }
}

Player& GameLogic::getPlayer(const std::string& username) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_players.at(username);
}

const std::vector<GameSession>& GameLogic::getActiveGames() const {
    return m_activeGames;
}

std::vector<Player> GameLogic::getAllPlayers() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<Player> result;
    for (const auto& [username, player] : m_players) {
        result.push_back(player);
    }
    return result;
}

GameLogic::CellType GameLogic::getCell(int row, int col) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (row < 0 || row >= m_mapHeight || col < 0 || col >= m_mapWidth) {
        return CellType::FREE;
    }
    return m_map[row][col];
}

const Player* GameLogic::getPlayerInfo(const std::string& username) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_players.find(username);
    if (it != m_players.end()) {
        return &(it->second); // Return a pointer to the Player
    }
    throw std::runtime_error("Player not found: " + username);
}


std::vector<std::string> GameLogic::getLobbyPlayers(int gameId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& game : m_activeGames) {
        if (game.id == gameId) {
            return game.players;
        }
    }
    return {};
}
