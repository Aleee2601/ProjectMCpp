#include "crow.h"
#include "GameSession.h"
#include "Player.h"
#include "Map.h"
#include "Bullet.h"

GameSession session;
Map gameMap(10, 10);

int main() {
    crow::SimpleApp app;

    // Adaugarea unui jucator nou
    CROW_ROUTE(app, "/add_player").methods("POST"_method)([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid JSON format!");

            Player newPlayer(body["id"].i(), body["name"].s(), body["x"].i(), body["y"].i());
            session.AddPlayer(newPlayer);

            return crow::response(200, "Player added successfully!");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });

    // Lista cu jucători
    CROW_ROUTE(app, "/get_players").methods("GET"_method)([]() {
        try {
            auto players = session.GetAllPlayers();
            crow::json::wvalue jsonResponse;
            jsonResponse["players"] = crow::json::wvalue::list();

            for (const auto& player : players) {
                crow::json::wvalue playerJson;
                playerJson["id"] = player.GetId();
                playerJson["name"] = player.GetName();
                playerJson["x"] = player.GetX();
                playerJson["y"] = player.GetY();
                jsonResponse["players"][jsonResponse["players"].size()] = std::move(playerJson);
            }
            return crow::response(200, jsonResponse);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }        });

    // Actualizarea poziției unui jucător
    CROW_ROUTE(app, "/update_position").methods("PUT"_method)([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid JSON format!");

            int playerId = body["id"].i();
            int newX = body["x"].i();
            int newY = body["y"].i();

            bool updated = session.UpdatePlayerPosition(playerId, newX, newY);
            if (!updated) {
                return crow::response(404, "Player not found!");
            }

            return crow::response(200, "Player position updated!");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });

    // Endpoint pentru logica bullet
    CROW_ROUTE(app, "/bullet").methods("POST"_method)([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid JSON format!");

            int startX = body["startX"].i();
            int startY = body["startY"].i();
            Direction dir = static_cast<Direction>(body["direction"].i());
            Bullet bullet(startX, startY, dir);

            bullet.Move();
            if (bullet.IsOutOfBounds(gameMap.GetWidth(), gameMap.GetHeight())) {
                bullet.LogOutOfBounds(gameMap.GetWidth(), gameMap.GetHeight());
                return crow::response(200, "Bullet out of bounds!");
            }

            for (const auto& player : session.GetAllPlayers()) {
                if (bullet.DetectCollision(player)) {
                    return crow::response(200, "Bullet hit player " + player.GetName() + "!");
                }
            }

            return crow::response(200, "Bullet moved without hitting anything.");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });

    // Endpoint pentru vizualizarea hărții
    CROW_ROUTE(app, "/map")([](const crow::request& req) {
        std::string acceptHeader = req.get_header_value("Accept");

        if (acceptHeader.find("text/html") != std::string::npos) {
            // Răspuns pentru browser: HTML formatat
            std::string html = "<pre>"; // Folosim <pre> pentru format lizibil
            for (int i = 0; i < gameMap.GetHeight(); ++i) {
                for (int j = 0; j < gameMap.GetWidth(); ++j) {
                    switch (gameMap.GetCellType(i, j)) {
                    case CellType::EMPTY: html += "."; break;
                    case CellType::DESTRUCTIBLE_WALL: html += "D"; break;
                    case CellType::INDESTRUCTIBLE_WALL: html += "I"; break;
                    }
                }
                html += "\n";
            }
            html += "</pre>";
            return crow::response(html);
        }
        else {
            // Răspuns pentru aplicație: JSON structurat
            crow::json::wvalue jsonResponse;
            crow::json::wvalue::list grid;

            for (int i = 0; i < gameMap.GetHeight(); ++i) {
                crow::json::wvalue::list row;
                for (int j = 0; j < gameMap.GetWidth(); ++j) {
                    switch (gameMap.GetCellType(i, j)) {
                    case CellType::EMPTY: row.emplace_back("."); break;
                    case CellType::DESTRUCTIBLE_WALL: row.emplace_back("D"); break;
                    case CellType::INDESTRUCTIBLE_WALL: row.emplace_back("I"); break;
                    }
                }
                grid.emplace_back(std::move(row));
            }
            jsonResponse["grid"] = std::move(grid);
            return crow::response(jsonResponse);
        }
        });

    std::cout << "Server is running on http://localhost:18080" << std::endl;

    app.port(18080).multithreaded().run();
}
