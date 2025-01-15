//#include "GameServer.h"
//#include "Map.h"
//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <vector>
//
//// Constructor
//GameServer::GameServer(int port)
//    : m_port(port)
//{
//    // Inițializăm harta curentă
//    m_currentMap = std::make_shared<Map>(15, 15);
//    m_currentMap->genereazaHartaRandom();
//
//    // Configurăm rutele serverului
//    initRoutes();
//}
//
//void GameServer::initRoutes()
//{
//    // Endpoint de test
//    CROW_ROUTE(m_app, "/")
//        ([] {
//        return "Bine ai venit la Battle City GameServer!";
//            });
//
//    // Endpoint Register
//    CROW_ROUTE(m_app, "/register").methods(crow::HTTPMethod::POST)
//        ([this](const crow::request& req) {
//        auto username = req.url_params.get("username");
//        if (!username) {
//            return crow::response(400, "Lipseste parametru 'username'.\n");
//        }
//
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        bool ok = registerUser(username);
//        if (!ok) {
//            return crow::response(409, "Nume de utilizator deja existent!\n");
//        }
//
//        return crow::response(200, "Inregistrare reusita!\n");
//            });
//
//    // Endpoint Login
//    CROW_ROUTE(m_app, "/login")
//        ([this](const crow::request& req) {
//        auto username = req.url_params.get("username");
//        if (!username) {
//            return crow::response(400, "Lipseste parametru 'username'.\n");
//        }
//
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        if (!userExists(username)) {
//            return crow::response(404, "Utilizatorul nu exista!\n");
//        }
//
//        // Pune userul în coadă
//        queuePlayer(username);
//
//        return crow::response(200, "Logare reusita! Userul a fost pus in coada de meci.\n");
//            });
//
//    // Endpoint status
//    CROW_ROUTE(m_app, "/status")
//        ([this]() {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        std::string resp;
//        resp += "Numar meciuri active: " + std::to_string(m_activeGames.size()) + "\n";
//        resp += "Jucatori in coada: " + std::to_string(m_playerQueue.size()) + "\n";
//        return crow::response(200, resp);
//            });
//
//    // Endpoint simplu de updateAll
//    CROW_ROUTE(m_app, "/updateAll")
//        ([this]() {
//        std::lock_guard<std::mutex> lock(m_mutex);
//        for (auto& game : m_activeGames)
//        {
//            if (!game->isGameOver())
//            {
//                game->update();
//            }
//        }
//        return crow::response(200, "Am facut update la toate meciurile.\n");
//            });
//
//    // Endpoint pentru a obține harta curentă
//    CROW_ROUTE(m_app, "/currentMap")
//        ([this]() {
//        if (!m_currentMap) {
//            return crow::response(404, "Nu exista o harta curenta.\n");
//        }
//
//        crow::json::wvalue mapJson;
//        mapJson["height"] = m_currentMap->getHeight();
//        mapJson["width"] = m_currentMap->getWidth();
//
//        std::vector<std::vector<std::string>> cells;
//        for (int i = 0; i < m_currentMap->getHeight(); ++i) {
//            std::vector<std::string> row;
//            for (int j = 0; j < m_currentMap->getWidth(); ++j) {
//                switch (m_currentMap->getCellType(i, j)) {
//                case CellType::FREE:
//                    row.push_back("FREE");
//                    break;
//                case CellType::BREAKABLE:
//                    row.push_back("BREAKABLE");
//                    break;
//                case CellType::UNBREAKABLE:
//                    row.push_back("UNBREAKABLE");
//                    break;
//                }
//            }
//            cells.push_back(std::move(row));
//        }
//
//        mapJson["cells"] = cells;
//
//        return crow::response(mapJson);
//            });
//
//    // Endpoint breakWall
//    CROW_ROUTE(m_app, "/breakWall").methods(crow::HTTPMethod::POST)
//        ([this](const crow::request& req) {
//        auto x = req.url_params.get("x");
//        auto y = req.url_params.get("y");
//        if (!x || !y)
//        {
//            return crow::response(400, "Parametrii 'x' si 'y' lipsesc.\n");
//        }
//
//        int ix = std::stoi(x);
//        int iy = std::stoi(y);
//
//        if (!m_currentMap->isValidPosition(ix, iy))
//        {
//            return crow::response(400, "Coordonate invalide.\n");
//        }
//
//        if (m_currentMap->breakWall(ix, iy))
//        {
//            return crow::response(200, "Zid spart cu succes!\n");
//        }
//        else
//        {
//            return crow::response(400, "Nu exista un zid destructibil la acea pozitie.\n");
//        }
//            });
//
//    // Endpoint shoot
//    CROW_ROUTE(m_app, "/shoot").methods(crow::HTTPMethod::POST)
//        ([this](const crow::request& req) {
//        auto username = req.url_params.get("username");
//        auto directionX = req.url_params.get("dx");
//        auto directionY = req.url_params.get("dy");
//
//        if (!username || !directionX || !directionY) {
//            return crow::response(400, "Lipsesc parametrii 'username', 'dx', sau 'dy'.\n");
//        }
//
//        double dx = std::stod(directionX);
//        double dy = std::stod(directionY);
//
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        // Găsim meciul jucătorului
//        for (auto& game : m_activeGames) {
//            if (game->hasPlayer(username)) {
//                game->shootBullet(username, dx, dy);
//                return crow::response(200, "Glonț tras cu succes!\n");
//            }
//        }
//
//        return crow::response(404, "Jucătorul nu este într-un meci activ.\n");
//            });
//
//    // Endpoint bullets
//    CROW_ROUTE(m_app, "/bullets")
//        ([this]() {
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        crow::json::wvalue bulletsJson;
//        int gameIndex = 0;
//        for (const auto& game : m_activeGames) {
//            bulletsJson["games"][std::to_string(gameIndex)] = game->getBulletsState();
//            gameIndex++;
//        }
//
//        return crow::response(bulletsJson);
//            });
//
//    CROW_ROUTE(m_app, "/gameState")
//        ([this]() {
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        // Într-un joc real, clientul ar trebui să specifice meciul sau username-ul
//        // pentru care cere starea. Deocamdată, returnăm starea primului meci activ.
//        if (m_activeGames.empty())
//            return crow::response(404, "Nu există jocuri active.\n");
//
//        // Să luăm primul meci
//        auto& game = m_activeGames[0];
//
//        auto stateJson = game->getGameStateJson();
//        // Metodă nouă pe care o definești în `GameLogic` => returnează un JSON cu:
//        //   - jucători (poziție, vieți)
//        //   - gloanțe (x, y)
//        //   - optional, winner, etc.
//
//        return crow::response(stateJson);
//            });
//
//    CROW_ROUTE(m_app, "/move").methods(crow::HTTPMethod::POST)
//        ([this](const crow::request& req) {
//        auto username = req.url_params.get("username");
//        auto dxStr = req.url_params.get("dx");
//        auto dyStr = req.url_params.get("dy");
//
//        if (!username || !dxStr || !dyStr)
//            return crow::response(400, "Lipsesc parametrii 'username', 'dx', 'dy'.\n");
//
//        int dx = std::stoi(dxStr);
//        int dy = std::stoi(dyStr);
//
//        std::lock_guard<std::mutex> lock(m_mutex);
//
//        // Căutăm meciul jucătorului
//        for (auto& game : m_activeGames)
//        {
//            if (game->hasPlayer(username))
//            {
//                // Aici facem logica: game->movePlayer(username, dx, dy);
//                game->movePlayer(username, dx, dy);
//
//                // (Opțional) Facem un update la joc: game->update();
//                // astfel încât gloanțele să se miște, coliziunile să se verifice etc.
//                game->update();
//
//                return crow::response(200, "Move efectuat.\n");
//            }
//        }
//
//        return crow::response(404, "Jucătorul nu este într-un meci activ.\n");
//            });
//
//}
//
//// Pornirea serverului (blochează execuția pe acest thread)
//void GameServer::run()
//{
//    std::cout << "GameServer ruleaza pe portul " << m_port << "...\n";
//    m_app.port(m_port).multithreaded().run();
//}
//
//// Verificare user
//bool GameServer::userExists(const std::string& username)
//{
//    return (m_users.find(username) != m_users.end());
//}
//
//// Înregistrare user
//bool GameServer::registerUser(const std::string& username)
//{
//    if (userExists(username)) {
//        return false;
//    }
//    m_users[username] = "user_info_placeholder";
//    return true;
//}
//
//// Pune jucătorul în coadă
//void GameServer::queuePlayer(const std::string& username)
//{
//    for (const auto& u : m_playerQueue)
//    {
//        if (u == username) {
//            return;
//        }
//    }
//    m_playerQueue.push_back(username);
//
//    // Verificăm dacă putem începe un meci
//    maybeStartGame();
//}
//
//// Pornește un meci nou dacă avem suficienți jucători
//void GameServer::maybeStartGame()
//{
//    constexpr int PLAYERS_PER_GAME = 4;
//
//    while (m_playerQueue.size() >= PLAYERS_PER_GAME)
//    {
//        auto mapPtr = std::make_shared<Map>(15, 20);
//        auto gameLogic = std::make_unique<GameLogic>(mapPtr);
//
//        // Adăugăm 4 jucători
//        for (int i = 0; i < PLAYERS_PER_GAME; i++)
//        {
//            std::string username = m_playerQueue.front();
//            m_playerQueue.erase(m_playerQueue.begin());
//
//            auto playerPtr = std::make_unique<Player>(username, 0, 0);
//            gameLogic->addPlayer(std::move(playerPtr));
//        }
//
//        // Plasăm jucătorii în colțurile hărții
//        gameLogic->initializePlayersInCorners();
//
//        // Adăugăm meciul în lista de meciuri active
//        m_activeGames.push_back(std::move(gameLogic));
//    }
//}
