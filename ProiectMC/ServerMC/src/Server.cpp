#include "../include/Server.h"
#include <iostream>
#include "../include/db/PlayerDAO.h"
#include "../include/Map.h"
#include "../include/db/DBPlayer.h"


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
    //CROW_ROUTE(m_app, "/register").methods(crow::HTTPMethod::POST)
    //    ([this](const crow::request& req) {
    //    std::cout << "Received register request: " << req.body << std::endl;
    //    try {
    //        // Parse JSON body
    //        auto body = crow::json::load(req.body);
    //        if (!body || !body.has("username") || !body.has("password")) {
    //            return crow::response(400, "Invalid JSON or missing fields.");
    //        }

    //        std::string username = body["username"].s();
    //        std::string password = body["password"].s();

    //        std::lock_guard<std::mutex> lock(m_mutex);
    //        if (userExists(username)) {
    //            return crow::response(409, "Username already exists!");
    //        }

    //        // Register user in the database
    //       /* if (!PlayerDAO().insertPlayer(username, password)) {
    //            return crow::response(500, "Error registering user.");
    //        }*/

    //        return crow::response(200, "Registration successful!");
    //    }
    //    catch (const std::exception& e) {
    //        return crow::response(500, std::string("Server error: ") + e.what());
    //    }
    //        });
    // Înregistrare utilizatori
    CROW_ROUTE(m_app, "/register").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        std::cout << "Received register request: " << req.body << std::endl;
        try {
            // Parse JSON body
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password")) {
                return crow::response(400, "Invalid JSON or missing fields.");
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();

            std::lock_guard<std::mutex> lock(m_mutex);
            if (userExists(username)) {
                return crow::response(409, "Username already exists!");
            }

            
            DBPlayer result = PlayerDAO().insertPlayer(username, password);
            if (&result == nullptr) {
                return crow::response(500, "Error registering user.");
            }

            return crow::response(200, "Registration successful!");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Server error: ") + e.what());
        }
            });

    // Login utilizatori
    CROW_ROUTE(m_app, "/login").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        std::cout << "Received login request: " << req.body << std::endl;
        try {
            // Parse JSON body
            auto body = crow::json::load(req.body);
            if (!body || !body.has("username") || !body.has("password")) {
                return crow::response(400, "Invalid JSON or missing fields.");
            }

            std::string username = body["username"].s();
            std::string password = body["password"].s();

            // Verify credentials
            std::lock_guard<std::mutex> lock(m_mutex);
            DBPlayer player = PlayerDAO().loginPlayer(username, password);
            if (player == nullptr)
            {
                return crow::response(401, "Invalid username or password.");
            }

            return crow::response(200, "Login successful.");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Server error: ") + e.what());
        }
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
        state["height"] = m_currentMap->GetHeight();
        state["width"] = m_currentMap->GetWidth();

        // Adăugăm structura hărții în JSON folosind o funcție lambda
        auto serializeMap = [this]() {
            crow::json::wvalue::list mapData;
            for (int i = 0; i < m_currentMap->GetHeight(); ++i) {
                crow::json::wvalue::list row;
                for (int j = 0; j < m_currentMap->GetWidth(); ++j) {
                    row.emplace_back(static_cast<int>(m_currentMap->GetCellType(i, j)));
                }
                mapData.emplace_back(std::move(row));
            }
            return mapData;
            };
        state["map"] = serializeMap();

        // Adăugăm informațiile jucătorilor în JSON
        auto serializePlayers = [this]() {
            crow::json::wvalue::list playersList;
            for (const auto& player : m_gameSession->GetAllPlayers()) {
                crow::json::wvalue playerInfo;
                playerInfo["id"] = player.GetId();
                playerInfo["name"] = player.GetName();
                playerInfo["x"] = player.GetX();
                playerInfo["y"] = player.GetY();
                playerInfo["lives"] = player.GetLives();  // Adaugă viețile
                playerInfo["kills"] = player.GetKills();  // Adaugă inamicii doborâți

                playersList.emplace_back(std::move(playerInfo));
            }
            return playersList;
            };
        state["players"] = serializePlayers();

        // Adăugăm informațiile despre bombe dacă există
        auto serializeBombs = [this]() {
            crow::json::wvalue::list bombsList;
            auto bombPositions = m_currentMap->GetBombPositions();
            auto bombRadii = m_currentMap->GetBombRadii();

            for (size_t i = 0; i < bombPositions.size(); ++i) {
                crow::json::wvalue bombInfo;
                bombInfo["x"] = bombPositions[i].first;
                bombInfo["y"] = bombPositions[i].second;
                bombInfo["radius"] = bombRadii[i];
                bombsList.emplace_back(std::move(bombInfo));
            }
            return bombsList;
            };
        state["bombs"] = serializeBombs();

        return crow::response(state);
            });

    CROW_ROUTE(m_app, "/updateMap").methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("x") || !body.has("y") || !body.has("type")) {
            return crow::response(400, "Invalid JSON.");
        }

        int x = body["x"].i();
        int y = body["y"].i();
        int newType = body["type"].i();

        // Verificăm dacă coordonatele sunt valide
        if (m_currentMap->IsWithinBounds(x, y)) {
            // Actualizăm celula pe hartă în funcție de tipul primit
            auto currentType = m_currentMap->GetCellType(x, y);

            if (currentType == CellType::DESTRUCTIBLE_WALL && newType == static_cast<int>(CellType::EMPTY)) {
                m_currentMap->DestroyWall(x, y);
            }
            else if (newType == static_cast<int>(CellType::BOMB)) {
                std::vector<Player> players = m_gameSession->GetAllPlayers(); // Assume this fetches all players
                m_currentMap->ActivateBombIfNeeded(x, y, players);
            }
            else {
                m_currentMap->SetCellType(x, y, static_cast<CellType>(newType));
            }

            // Send updated map back to client as part of response
            crow::json::wvalue updatedMap;
            updatedMap["status"] = "success";
            updatedMap["updated_cell"] = { {"x", x}, {"y", y}, {"new_type", newType} };

            // Serialize the entire map after updates
            auto serializeMap = [this]() {
                crow::json::wvalue::list mapData;
                for (int i = 0; i < m_currentMap->GetHeight(); ++i) {
                    crow::json::wvalue::list row;
                    for (int j = 0; j < m_currentMap->GetWidth(); ++j) {
                        row.emplace_back(static_cast<int>(m_currentMap->GetCellType(i, j)));
                    }
                    mapData.emplace_back(std::move(row));
                }
                return mapData;
                };
            updatedMap["map"] = serializeMap();

            return crow::response(updatedMap);
        }

        return crow::response(400, "Invalid map coordinates.");
            });



    CROW_ROUTE(m_app, "/lobby").methods(crow::HTTPMethod::GET)([this]() {
        crow::json::wvalue response;
        crow::json::wvalue::list playersList;
        for (const auto& player : m_gameSession->GetAllPlayers()) {
            crow::json::wvalue playerInfo;
            playerInfo["id"] = player.GetId();
            playerInfo["name"] = player.GetName();
            playerInfo["image"] = player.GetImage(); // Imaginea jucătorului, dacă există
            playersList.emplace_back(std::move(playerInfo));
        }
        response["players"] = std::move(playersList);
        response["timeRemaining"] = m_gameSession->GetLobbyTimeRemaining(); // Timpul rămas
        return crow::response(response);
        });


    CROW_ROUTE(m_app, "/startGame").methods(crow::HTTPMethod::POST)([this]() {
        if (m_gameSession->CanStartGame()) {
            m_gameSession->StartGame();
            GameDAO.create(m_gameSession->GetAllPlayers());
            return crow::response(200, "Game started.");
        }
        return crow::response(400, "Not enough players or other issue.");
        });

    CROW_ROUTE(m_app, "/get_players")
        ([this]() {
        crow::json::wvalue response;
        crow::json::wvalue::list playersList;

       
        auto players = m_gameSession->GetAllPlayers();  

        for (const auto& player : players) {
            crow::json::wvalue playerInfo;
            playerInfo["id"] = player.GetId();
            playerInfo["name"] = player.GetName();
            playerInfo["x"] = player.GetX();
            playerInfo["y"] = player.GetY();
            playerInfo["lives"] = player.GetLives();  
            playerInfo["kills"] = player.GetKills();  
            playersList.emplace_back(std::move(playerInfo));  
        }

        response["players"] = std::move(playersList);  
        return crow::response(response);
            });

}



// Pornire server
void Server::run() {
    std::cout << "Server running on port " << m_port << "...\n";
    m_app.port(m_port).multithreaded().run();
}

// Verificare utilizator
bool Server::userExists(const std::string& username) {
    DBPlayer player = PlayerDAO().findPlayerByNickname(username);
    return player.isValid(); 
}

// Înregistrare utilizator
bool Server::registerUser(const std::string& username) {
    if (userExists(username)) return false;
    m_users[username] = "placeholder_info";
    DBPlayer player = PlayerDAO().insertPlayer(username, "");
    return true;
}

// Adăugare utilizator în coadă
void Server::queuePlayer(const std::string& username) {
    if (std::find(m_playerQueue.begin(), m_playerQueue.end(), username) == m_playerQueue.end()) {
        DBPlayer player = PlayerDAO().findPlayerByNickname(username);
        if (userExists(username)) return;
        m_playerQueue.push_back(player.GetName());
        maybeStartGame();
    }
}

// Start joc dacă avem suficienți jucători
void Server::maybeStartGame() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_playerQueue.size() >= 4) { 
        auto newGameSession = std::make_unique<GameSession>(m_currentMap);
        for (int i = 0; i < 4; ++i) {
            std::string player = m_playerQueue.back();
            m_playerQueue.pop_back();
            //newGameSession->AddPlayer(Player(i, player, 0, 0, Direction::UP, "default_image_path"));
        }
        m_activeGames.push_back(std::move(newGameSession));
    }
}
