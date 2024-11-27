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
void addPlayer() {
    int id, x, y;
    std::string name;

    std::cout << "Enter player ID: ";
    std::cin >> id;
    std::cout << "Enter player name: ";
    std::cin >> name;
    std::cout << "Enter X position: ";
    std::cin >> x;
    std::cout << "Enter Y position: ";
    std::cin >> y;

    auto response = cpr::Post(
        cpr::Url{ "http://localhost:8080/add_player" },
        cpr::Body{ "{\"id\":" + std::to_string(id) +
                  ",\"name\":\"" + name +
                  "\",\"x\":" + std::to_string(x) +
                  ",\"y\":" + std::to_string(y) + "}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << "Player added successfully!\n";
    }
    else {
        std::cerr << "Error adding player. Status code: " << response.status_code << '\n';
    }
}

void updatePlayerPosition() {
    int id, x, y;

    std::cout << "Enter player ID: ";
    std::cin >> id;
    std::cout << "Enter new X position: ";
    std::cin >> x;
    std::cout << "Enter new Y position: ";
    std::cin >> y;

    auto response = cpr::Put(
        cpr::Url{ "http://localhost:8080/update_position" },
        cpr::Body{ "{\"id\":" + std::to_string(id) +
                  ",\"x\":" + std::to_string(x) +
                  ",\"y\":" + std::to_string(y) + "}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );

    if (response.status_code == 200) {
        std::cout << "Player position updated successfully!\n";
    }
    else {
        std::cerr << "Error updating player position. Status code: " << response.status_code << '\n';
    }
}

void viewMap() {
    auto response = cpr::Get(cpr::Url{ "http://localhost:8080/map" });

    if (response.status_code == 200) {
        std::cout << "Here is the map:\n";

        // Parsează JSON-ul primit de la server
        auto mapJson = crow::json::load(response.text);
        if (!mapJson) {
            std::cerr << "Error parsing map JSON.\n";
            return;
        }

        // Extrage grid-ul din JSON
        auto grid = mapJson["grid"];
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell.s() << ' '; // Afișează fiecare celulă cu spațiu între ele
            }
            std::cout << '\n'; // Trece la următorul rând
        }
    }
    else {
        std::cerr << "Error fetching map. Status code: " << response.status_code << '\n';
    }
}

int main() {
    int choice = 0;

    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. List Players\n";
        std::cout << "2. Add Player\n";
        std::cout << "3. Update Player Position\n";
        std::cout << "4. View Map\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            listPlayers();
            break;
        case 2:
            addPlayer();
            break;
        case 3:
            updatePlayerPosition();
            break;
        case 4:
            viewMap();
            break;
        case 0:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cerr << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}