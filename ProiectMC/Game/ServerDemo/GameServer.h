//#pragma once
//
//#include <map>
//#include <string>
//#include <mutex>
//#include <vector>
//#include <memory>
//#include "crow.h"
////#include "crow_all.h"
//#include "GameLogic.h"
//#include "Map.h"
//
//class GameServer
//{
//public:
//    explicit GameServer(int port);
//    void run();
//
//private:
//    void initRoutes();
//    bool userExists(const std::string& username);
//    bool registerUser(const std::string& username);
//    void queuePlayer(const std::string& username);
//    void maybeStartGame();
//
//private:
//    crow::SimpleApp m_app;
//    int m_port;
//    std::map<std::string, std::string> m_users;
//    std::vector<std::string> m_playerQueue;
//    std::vector<std::unique_ptr<GameLogic>> m_activeGames;
//    std::mutex m_mutex;
//
//    // Harta curentă (ex. pentru debugging)
//    std::shared_ptr<Map> m_currentMap;
//};
