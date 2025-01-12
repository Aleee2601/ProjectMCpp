#include "include/Server.h"
#include <iostream>
#include <string>
#include <crow.h>
#include <nlohmann/json.hpp>
#include "GameSession.h"
#include "Map.h"

Server::Server(unsigned short port) : gameSession(), gameMap(10, 10) {
    setupRoutes();
    app.port(port).multithreaded().run();
}

void Server::setupRoutes() {
    app.route_dynamic("/move").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
        try {
            auto jsonMessage = nlohmann::json::parse(req.body);

            int playerId = jsonMessage["playerId"].get<int>();
            std::string direction = jsonMessage["direction"].get<std::string>();
            handleMoveCommand(playerId, direction);

            return crow::response(200);
        }
        catch (const std::exception& e) {
            std::cerr << "Error handling move command: " << e.what() << std::endl;
            return crow::response(400, "Invalid request");
        }
        });

    app.route_dynamic("/broadcast_position").methods(crow::HTTPMethod::GET)([this]() {
        try {
            nlohmann::json responseJson;

            for (const auto& player : gameSession.GetAllPlayers()) {
                responseJson["players"].push_back({
                    {"id", player.GetId()},
                    {"x", player.GetX()},
                    {"y", player.GetY()}
                    });
            }

            return crow::response(responseJson.dump());
        }
        catch (const std::exception& e) {
            std::cerr << "Error broadcasting position: " << e.what() << std::endl;
            return crow::response(500);
        }
        });
}

void Server::handleMoveCommand(int playerId, const std::string& direction) {
    try {
        Player& player = gameSession.GetPlayerById(playerId);

        if (direction == "UP") {
            player.Move(Direction::UP, gameMap);
        }
        else if (direction == "DOWN") {
            player.Move(Direction::DOWN, gameMap);
        }
        else if (direction == "LEFT") {
            player.Move(Direction::LEFT, gameMap);
        }
        else if (direction == "RIGHT") {
            player.Move(Direction::RIGHT, gameMap);
        }

        std::cout << "Player " << playerId << " moved to position (" << player.GetX() << ", " << player.GetY() << ")" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error handling move command: " << e.what() << std::endl;
    }
}
