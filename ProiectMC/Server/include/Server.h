#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include "GameSession.h"
#include "Map.h"

class Server {
public:
    explicit Server(unsigned short port);
    void run();
    void handleMoveCommand(int playerId, const std::string& direction);
    void broadcastPlayerPosition(int playerId);

private:
    void setupRoutes();

    crow::SimpleApp app;
    GameSession gameSession;
    Map gameMap;
};
