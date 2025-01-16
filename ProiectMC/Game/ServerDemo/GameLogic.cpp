#include "GameLogic.h"
#include <iostream>

// ----------------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------------
GameLogic::GameLogic()
    : m_mapWidth(27)
    , m_mapHeight(12)
{
    initializeMap();
}

GameLogic::~GameLogic()
{
    // destructor simplu
}

// ----------------------------------------------------------------------------
// Register / Login
// ----------------------------------------------------------------------------
bool GameLogic::doRegister(const std::string& username, const std::string& password)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (username.empty() || password.empty())
    {
        std::cerr << "[Server] Register eșuat: username/parola goale.\n";
        return false;
    }

    if (m_userDatabase.find(username) != m_userDatabase.end())
    {
        std::cerr << "[Server] Register eșuat: user deja existent.\n";
        return false;
    }

    // Creăm un user nou
    m_userDatabase[username] = password;
    std::cout << "[Server] Înregistrare reușită pentru user: " << username << "\n";
    return true;
}

bool GameLogic::doLogin(const std::string& username, const std::string& password)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_userDatabase.find(username);
    if (it != m_userDatabase.end() && it->second == password)
    {
        std::cout << "[Server] Login reușit pentru user: " << username << "\n";

        // Dacă jucătorul nu există încă în joc, îl creăm și îl plasăm undeva
        if (m_players.find(username) == m_players.end())
        {
            // Creează un nou Player
            m_players[username] = Player(username, 1, 1); // Poziția inițială
        }

        return true;
    }

    std::cerr << "[Server] Login eșuat pentru user: " << username << "\n";
    return false;
}

// ----------------------------------------------------------------------------
// Joc
// ----------------------------------------------------------------------------
bool GameLogic::doStartGame()
{
    // Aici poți implementa reguli dacă ai nevoie 
    // (ex. să fie minim 2 useri logați etc.)
    std::cout << "[Server] Jocul a pornit!\n";
    return true;
}

bool GameLogic::movePlayer(const std::string& username, int dx, int dy)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_players.find(username);
    if (it == m_players.end())
    {
        std::cerr << "[Server] movePlayer eșuat: user inexistent.\n";
        return false;
    }

    Player& player = it->second;
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    // Verificăm limite
    if (newX < 0 || newX >= m_mapWidth || newY < 0 || newY >= m_mapHeight)
    {
        return false;
    }

    // Verificăm dacă e FREE
    if (m_map[newY][newX] == CellType::FREE)
    {
        player.move(dx, dy);
        std::cout << "[Server] " << username << " s-a mișcat la (" << newX << "," << newY << ")\n";
        return true;
    }

    return false;
}

void GameLogic::shoot(const std::string& username)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    // Logica de "împușcare" - creezi un glonț, îl pui într-un vector de gloanțe, etc.
    // Poți implementa coliziuni, damage, etc.
    std::cout << "[Server] " << username << " a tras un glonț!\n";
}

// Apelat periodic (ex. la un timer) pentru actualizarea stării (poziția gloanțelor, coliziuni, etc.)
void GameLogic::updateGameState()
{
    // De exemplu: actualizezi gloanțele, distrugi cărămizi, etc.
}

// ----------------------------------------------------------------------------
// Inițializare hartă
// ----------------------------------------------------------------------------
void GameLogic::initializeMap()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_map = std::vector<std::vector<CellType>>(m_mapHeight,
        std::vector<CellType>(m_mapWidth, CellType::FREE));

    for (int row = 0; row < m_mapHeight; ++row)
    {
        for (int col = 0; col < m_mapWidth; ++col)
        {
            if (row == 0 || row == m_mapHeight - 1 || col == 0 || col == m_mapWidth - 1)
            {
                m_map[row][col] = CellType::UNBREAKABLE;
            }
            else if ((col >= 10 && col <= 16) && (row >= 1 && row <= 3 || row >= 8 && row <= 10))
            {
                m_map[row][col] = CellType::UNBREAKABLE;
            }
            else
            {
                m_map[row][col] = CellType::FREE;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// Gettere
// ----------------------------------------------------------------------------
GameLogic::CellType GameLogic::getCell(int row, int col) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (row < 0 || row >= m_mapHeight || col < 0 || col >= m_mapWidth)
        return CellType::FREE; // fallback
    return m_map[row][col];
}

bool GameLogic::getPlayerPosition(const std::string& username, int& outX, int& outY) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_players.find(username);
    if (it != m_players.end())
    {
        outX = it->second.getX();
        outY = it->second.getY();
        return true;
    }
    return false;
}

std::vector<Player> GameLogic::getAllPlayers() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<Player> result;
    for (const auto& [username, player] : m_players) {
        result.push_back(player);
    }
    return result;
}
