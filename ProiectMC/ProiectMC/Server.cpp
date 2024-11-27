#include "crow.h"
#include "GameSession.h"
#include "ActiveClientGuard.h"
#include "map.h"


GameSession session;
Map gameMap(10, 10);

int main() {
    crow::SimpleApp app;

    std::atomic<int> active_clients{ 0 };
    const int MAX_CLIENTS = 4;


    // Adaugarea unui jucator nou
    CROW_ROUTE(app, "/add_player").methods("POST"_method)([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid JSON format!");

            Player newPlayer(body["id"].i(), body["name"].s(), body["x"].i(), body["y"].i());
            session.addPlayer(newPlayer);

            return crow::response(200, "Player added successfully!");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });
    CROW_ROUTE(app, "/")([]() {
        return "Welcome to the server! This is the root endpoint.";
        });

    // Lista cu jucatori
    CROW_ROUTE(app, "/get_players").methods("GET"_method)([]() {
        try {
            auto players = session.getAllPlayers();
            crow::json::wvalue jsonResponse;
            jsonResponse["players"] = crow::json::wvalue::list();

            for (const auto& player : players) {
                crow::json::wvalue playerJson;
                playerJson["id"] = player.getId();

                playerJson["name"] = player.getName();
                playerJson["x"] = player.getX();
                playerJson["y"] = player.getY();
                jsonResponse["players"][jsonResponse["players"].size()] = std::move(playerJson);
            }
            return crow::response(200, jsonResponse);
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });


    // Actualizarea poziției unui jucător
    CROW_ROUTE(app, "/update_position").methods("PUT"_method)([](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body)
                return crow::response(400, "Invalid JSON format!");

            int playerId = body["id"].i();
            int newX = body["x"].i();
            int newY = body["y"].i();

            bool updated = session.updatePlayerPosition(playerId, newX, newY);
            if (!updated) {
                return crow::response(404, "Player not found!");
            }


            return crow::response(200, "Player position updated!");
        }
        catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
        });

    CROW_ROUTE(app, "/map")([](const crow::request& req) {
        // Verificăm ce tip de răspuns așteaptă clientul (browser sau aplicație)
        std::string acceptHeader = req.get_header_value("Accept");

        if (acceptHeader.find("text/html") != std::string::npos) {
            // Răspuns pentru browser: HTML formatat
            std::string html = "<pre>";  // Folosim <pre> pentru format lizibil
            for (int i = 0; i < gameMap.getHeight(); ++i) {
                for (int j = 0; j < gameMap.getWidth(); ++j) {
                    switch (gameMap.getCellType(i, j)) {
                    case CellType::EMPTY: html += "."; break;
                    case CellType::DESTRUCTIBLE_WALL: html += "D"; break;
                    case CellType::INDESTRUCTIBLE_WALL: html += "I"; break;
                    }
                }
                html += "\n";  // Sfârșitul unui rând
            }
            html += "</pre>";
            return crow::response(html);
        }
        else {
            // Răspuns pentru aplicație: JSON structurat
            crow::json::wvalue jsonResponse;
            crow::json::wvalue::list grid;

            for (int i = 0; i < gameMap.getHeight(); ++i) {
                crow::json::wvalue::list row;
                for (int j = 0; j < gameMap.getWidth(); ++j) {
                    switch (gameMap.getCellType(i, j)) {
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

    std::cout << "Server is running on http://localhost:8080" << std::endl;

    app.port(8080).multithreaded().run();
}
