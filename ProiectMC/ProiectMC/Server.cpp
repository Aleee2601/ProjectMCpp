#include "crow.h"
#include "GameSession.h"

GameSession session;

int main() {
    crow::SimpleApp app;


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




    app.port(8080).multithreaded().run();
}
