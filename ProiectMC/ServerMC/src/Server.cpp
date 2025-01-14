#include "../include/Server.h"
#include <iostream>

// Constructor
//Server::Server(int port)
//    : m_port(port), m_currentMap(std::make_shared<Map>(15, 15)) {
//    m_currentMap->GenerateRandomMap(); // Generăm harta inițial
//    initRoutes();
//}
Server::Server(int port)
    : m_port(port), m_currentMap(std::make_shared<Map>(15, 15)),
    m_gameSession(std::make_unique<GameSession>(m_currentMap)) {
    m_currentMap->GenerateRandomMap();
    initRoutes();
}



// Inițializarea rutelor Crow
void Server::initRoutes() {
    // Răspuns pentru ruta principală
    CROW_ROUTE(m_app, "/")
        ([] {
        return "Welcome to Battle City Server!";
            });

    // Înregistrare utilizatori
    CROW_ROUTE(m_app, "/register").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        auto username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing 'username' parameter.\n");
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        if (!registerUser(username)) {
            return crow::response(409, "Username already exists!\n");
        }

        return crow::response(200, "Registration successful!\n");
            });

    // Login utilizatori
    CROW_ROUTE(m_app, "/login")
        ([this](const crow::request& req) {
        auto username = req.url_params.get("username");
        if (!username) {
            return crow::response(400, "Missing 'username' parameter.\n");
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        if (!userExists(username)) {
            return crow::response(404, "User does not exist!\n");
        }

        queuePlayer(username);
        return crow::response(200, "Login successful. Added to game queue.\n");
            });

    // Stare server
    CROW_ROUTE(m_app, "/status")
        ([this]() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::string resp = "Active games: " + std::to_string(m_activeGames.size()) + "\n";
        resp += "Players in queue: " + std::to_string(m_playerQueue.size()) + "\n";
        return crow::response(200, resp);
            });

    // Generarea hărții curente
    CROW_ROUTE(m_app, "/currentMap")
        ([this]() {
        if (!m_currentMap) {
            return crow::response(404, "No current map available.\n");
        }

        crow::json::wvalue state;
        crow::json::wvalue::list playersList;

        // Adaugăm informațiile jucătorilor
        for (const auto& player : m_gameSession->GetAllPlayers()) {
            crow::json::wvalue playerInfo;
            playerInfo["id"] = player.GetId();
            playerInfo["name"] = player.GetName();
            playerInfo["x"] = player.GetX();
            playerInfo["y"] = player.GetY();
            playersList.emplace_back(std::move(playerInfo));
        }

        // Adăugăm lista în JSON-ul final
        state["players"] = std::move(playersList);

        // Răspunsul JSON include și informații despre hartă
        state["map"]["height"] = m_currentMap->GetHeight();
        state["map"]["width"] = m_currentMap->GetWidth();

        return crow::response(state);
            });


}

// Pornire server
void Server::run() {
    std::cout << "Server running on port " << m_port << "...\n";
    m_app.port(m_port).multithreaded().run();
}

// Verificare utilizator
bool Server::userExists(const std::string& username) {
    return m_users.find(username) != m_users.end();
}

// Înregistrare utilizator
bool Server::registerUser(const std::string& username) {
    if (userExists(username)) return false;
    m_users[username] = "placeholder_info";
    return true;
}

// Adăugare utilizator în coadă
void Server::queuePlayer(const std::string& username) {
    if (std::find(m_playerQueue.begin(), m_playerQueue.end(), username) == m_playerQueue.end()) {
        m_playerQueue.push_back(username);
        maybeStartGame();
    }
}

// Start joc dacă avem suficienți jucători
void Server::maybeStartGame() {
    constexpr int PLAYERS_PER_GAME = 4;
    while (m_playerQueue.size() >= PLAYERS_PER_GAME) {
        auto gameSession = std::make_unique<GameSession>(m_currentMap);
        for (int i = 0; i < PLAYERS_PER_GAME; ++i) {
            auto username = m_playerQueue.front();
            m_playerQueue.erase(m_playerQueue.begin());
            //gameSession->AddPlayer(Player(username, 0, 0, Direction::UP))
            gameSession->AddPlayer(Player(1, "Player1", 0, 0, Direction::UP));
        }
        m_activeGames.push_back(std::move(gameSession));
    }
}
