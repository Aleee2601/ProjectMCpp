#include "crow_all.h"
#include "GameSession.h"

GameSession session;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/add_player").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        Player newPlayer(body["id"].i(), body["name"].s(), body["x"].i(), body["y"].i());
        session.addPlayer(newPlayer);
        return crow::response(200, "Player added!");
        });

    app.port(8080).multithreaded().run();
}
