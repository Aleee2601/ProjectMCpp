﻿#include "../include/GameSession.h"
#include <iostream>
#include <vector>

int main() {
    // Creăm câțiva jucători pentru test
    Player player1(1, "Player1", Direction::UP, "");
    Player player2(2, "Player2", Direction::DOWN, "");

    // Creăm o armă pentru jucători
    Weapon weapon1; // Cooldown 1 sec, viteza 10, damage 50
    Weapon weapon2;

    // Alocăm armele jucătorilor
    player1.AssignWeapon(weapon1);
    player2.AssignWeapon(weapon2);

    // Creăm o hartă
    Map gameMap(10, 10);  // Harta 10x10

    std::shared_ptr<Map> gameMapPtr = std::make_shared<Map>(gameMap);

    // Creăm sesiunea de joc, trecând gameMapPtr către constructorul GameSession
    GameSession gameSession(gameMapPtr);
    gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());
    gameSession.AddPlayer(player1);  // Adăugăm jucătorul 1
    gameSession.AddPlayer(player2);  // Adăugăm jucătorul 2
    gameSession.StartGame();
    // Testăm mutările și acțiunile
    char choice;
    bool gameRunning = true;

    while (gameRunning) {
        int playerId;
        std::cout << "Which player moves next? Enter ID: ";
        std::cin >> playerId;

        // Verificăm dacă jucătorul există
        bool playerFound = false;
        for (const auto& player : gameSession.GetAllPlayers()) {
            if (player.GetId() == playerId) {
                playerFound = true;
                break;
            }
        }

        if (!playerFound) {
            std::cout << "Player not found. Please try again.\n";
            continue;  // Dacă jucătorul nu există, continuăm cu următoarea iterație
        }

        // Afișăm meniul pentru mutarea jucătorului
        std::cout << "Choose the direction for your move (W - Up, A - Left, S - Down, D - Right, Q - Quit): ";
        std::cin >> choice;

        // Executăm acțiunea în funcție de alegerea utilizatorului
        Direction moveDirection = Direction::UP;  // Direcția implicită

        switch (choice) {
        case 'W': case 'w':
            moveDirection = Direction::UP;
            break;
        case 'A': case 'a':
            moveDirection = Direction::LEFT;
            break;
        case 'S': case 's':
            moveDirection = Direction::DOWN;
            break;
        case 'D': case 'd':
            moveDirection = Direction::RIGHT;
            break;
        case 'Q': case 'q':
            gameRunning = false;  // Oprirea jocului
            std::cout << "Game Over! You have quit the game." << std::endl;
            break;
        default:
            std::cout << "Invalid input. Please try again." << std::endl;
            break;
        }

        if (gameRunning) {
            // Executăm mișcarea
            gameSession.MovePlayer(playerId, moveDirection);

            // Afișăm harta actualizată
            gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());
            gameSession.DisplayGameState();

            // Opțiune pentru a trage cu arma
            std::cout << "Do you want to shoot? (Y/N): ";
            std::cin >> choice;

            if (choice == 'Y' || choice == 'y') {
                player1.Shoot(moveDirection, 0.5f);  // Exemplu: trage în dreapta
                gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());
                std::vector<Bullet> bullets = gameSession.GetAllBullets();

                std::cout << "Bullets in the game:" << std::endl;


                if (bullets.empty()) {
                    std::cout << "No bullets to display." << std::endl;
                }
                else {
                    for (size_t i = 0; i < bullets.size(); ++i) {
                        const Bullet& bullet = bullets[i];
                        std::cout << "Bullet " << i + 1 << ": "
                            << "Position [" << bullet.GetX() << ", " << bullet.GetY() << "], "
                            << "Direction: " << static_cast<int>(bullet.GetDirection()) << ", "
                            << "Active: " << (bullet.IsInactive() ? "No" : "Yes") << ", "
                            << "Owner ID: " << bullet.GetOwnerId()
                            << std::endl;
                    }
                }
            }

            // Poți adăuga alte opțiuni de acțiuni aici (de exemplu, distrugerea zidurilor, interacțiuni etc.)
        }
    }
    // Pornim jocul
    //gameSession.StartGame();

    //// Simulăm câteva acțiuni
    //gameSession.DisplayGameState();  // Arată starea jocului

    //// Testăm mutarea unui jucător
    //gameSession.DisplayGameState();  // Verificăm noua poziție

    //// Testăm distrugerea unui zid
    //gameMap.DestroyWallWithDisplay(5, 5, gameSession.GetAllPlayers());

    //// Testăm finalizarea jocului
    //gameSession.EndGame();


    return 0;
}