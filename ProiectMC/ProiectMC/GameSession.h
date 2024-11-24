#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "map.h"
#include "Player.h"
#include "Event.h"

class GameSession {
public:
    // Constructor pentru initializarea sesiunii de joc cu o harta de dimensiune n x m
    GameSession(int n, int m) : gameMap(n, m), currentTurn(0) {}

    // Adauga un jucator in sesiune
    void addPlayer(const Player& player);

    // Afiseaza starea curenta a jocului
    void displayGameState() const;

    // Simuleaza lovirea unui jucator de catre altul
    void playerHits(Player& shooter, Player& target);

    // Trecerea la urmatorul tur
    void nextTurn();

    // Obtine jucatorul curent care trebuie sa joace
    Player& getCurrentPlayer();

    // Verifica daca jucatorul curent a fost eliminat
    bool isCurrentPlayerEliminated();

    // Incepe turul curent
    void startTurn();

    // Actualizeaza pozitia unui jucator specificat prin ID
    void updatePlayerPosition(int playerId, int newX, int newY);

    // Returneaza o lista cu toti jucatorii conectati
    std::vector<Player> getAllPlayers() const;

    // Elimina un jucator specificat prin ID
    void removePlayerById(int playerId);

    std::vector<Player> GetAllPlayers() const;

    void recordEvent(std::unique_ptr<Event> event);
    void processEvents() const;

    //clasament
    void displayLeaderboard() const;
private:
    Map gameMap;                  // Harta jocului
    std::vector<Player> players;  // Lista de jucatori din sesiune
    int currentTurn;              // Indexul jucatorului curent
    std::vector<std::unique_ptr<Event>> events;
};
