#include "../include/GameSession.h"
#include <iostream>
#include <vector>

int main() {
    // Creăm câțiva jucători pentru test
    Player player1(1, "Player1", Direction::UP, "");
    Player player2(2, "Player2", Direction::DOWN, "");

    // Creăm o armă pentru jucători
    Weapon weapon1(1.0f, 10.0f, 50); // Cooldown 1 sec, viteza 10, damage 50
    Weapon weapon2(1.0f, 10.0f, 50);

    // Alocăm armele jucătorilor
    player1.AssignWeapon(weapon1);
    player2.AssignWeapon(weapon2);

    // Creăm o hartă
    Map gameMap(10, 10);  // Harta 10x10

    // Creăm sesiunea de joc
    GameSession gameSession(10, 10);
    gameSession.AddPlayer(player1);  // Adăugăm jucătorul 1
    gameSession.AddPlayer(player2);  // Adăugăm jucătorul 2

    // Pornim jocul
    gameSession.StartGame();

    // Simulăm câteva acțiuni
    gameSession.DisplayGameState();  // Arată starea jocului

    // Testăm mutarea unui jucător
    gameSession.DisplayGameState();  // Verificăm noua poziție

    // Testăm focul unei arme
    player1.Shoot(Direction::RIGHT);
    player2.Shoot(Direction::LEFT);

    // Testăm distrugerea unui zid
    gameMap.DestroyWallWithDisplay(5, 5, gameSession.GetAllPlayers());

    // Testăm finalizarea jocului
    gameSession.EndGame();

    return 0;
}