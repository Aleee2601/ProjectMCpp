#include "../include/GameSession.h"
#include <iostream>
#include <vector>
#include <chrono>
int main() {
    

    Player player1(1, "Player1", Direction::UP, "");
    Player player2(2, "Player2", Direction::DOWN, "");


    Weapon weapon1; 
    Weapon weapon2;

    player1.AssignWeapon(weapon1);
    player2.AssignWeapon(weapon2);

    Map gameMap(10, 10); 

    std::shared_ptr<Map> gameMapPtr = std::make_shared<Map>(gameMap);

    GameSession gameSession(gameMapPtr);
    gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());
    gameSession.AddPlayer(player1);  
    gameSession.AddPlayer(player2); 
    gameSession.StartGame();

    char choice;
    bool gameRunning = true;

    auto previous_time = std::chrono::high_resolution_clock::now(); 

    while (gameRunning) {
        
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed_time = current_time - previous_time;
        previous_time = current_time;
        float delta_time = elapsed_time.count(); 

        gameSession.MoveBullets(delta_time);
        int playerId;
        std::cout << "Which player moves next? Enter ID: ";
        std::cin >> playerId;

        bool playerFound = false;
        for (const auto& player : gameSession.GetAllPlayers()) {
            if (player.GetId() == playerId && !player.IsEliminated()) {
                playerFound = true;
                break;
            }
        }

        if (!playerFound) {
            std::cout << "Player not found. Please try again.\n";
            continue; 
        }

        std::cout << "Choose the direction for your move (W - Up, A - Left, S - Down, D - Right, Q - Quit): ";
        std::cin >> choice;

        Direction moveDirection = Direction::UP; 

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
            gameRunning = false;  
            std::cout << "Game Over! You have quit the game." << std::endl;
            break;
        default:
            std::cout << "Invalid input. Please try again." << std::endl;
            break;
        }

        if (gameRunning) {
            gameSession.MovePlayer(playerId, moveDirection);

            gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());
            gameSession.DisplayGameState();

            std::cout << "Do you want to shoot? (Y/N): ";
            std::cin >> choice;

            if (choice == 'Y' || choice == 'y') {
                Player* activePlayer = gameSession.GetPlayerById(playerId); 
                if (activePlayer) {
                    activePlayer->Shoot(moveDirection, delta_time);
                }
                else {
                    std::cout << "Error: Player not found in game session.\n";
                }

                gameMap.DisplayMap(gameSession.GetAllPlayers(), gameSession.GetAllBullets());


                std::vector<Player> players = gameSession.GetAllPlayers();
                if (players.empty()) {
                    std::cout << "No players in this round" << std::endl;
                }
                else
                    for (size_t i = 0; i < players.size(); ++i)
                    {
                        const Player& player = players[i];
                        std::cout << "Player " << i + 1 << ": "
                            << "Player ID: " << player.GetId() << ", "
                            << "Player status: " << player.PlayerStatusToString(player.GetStatus());
                    }

                std::vector<Bomb> bombs = gameMap.GetAllBombs();
                if (bombs.empty()) {
                    std::cout << "No bombs to display" << std::endl;
                }
                else
                    for (size_t i = 0; i < bombs.size(); ++i)
                    {
                        const Bomb& bomb = bombs[i];
                        std::cout << "Bomb " << i + 1 << ": "
                            << bomb.GetPosition().first <<" "<<bomb.GetPosition().second << std::endl;
                            
                    }
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

        }
    }

    return 0;
}