#include <iostream>
#include <regex>
#include <string>
#include <cpr/cpr.h>
#include <crow.h>

void listPlayers() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:8080/get_players" });
    if (response.status_code == 200) {
        std::cout << "Here is the list of players:\n";

        auto players = crow::json::load(response.text);

        if (!players) {
            std::cerr << "Error parsing JSON from response.\n";
            return;
        }

        for (const auto& player : players["players"]) {  // Accesam lista "players" din JSON
            std::cout << "ID: " << player["id"] << '\t'
                << "Name: " << player["name"].s() << '\t'
                << "Position: (" << player["x"] << ", " << player["y"] << ")\n";
        }

    }
    else {
        std::cerr << "Error fetching players. Status code: " << response.status_code << '\n';
    }
}