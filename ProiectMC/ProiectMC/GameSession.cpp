#include "GameSession.h"
#include "Player.h"
#include <iostream>
#include"Event.h"
void GameSession::displayGameState() const {
	std::cout << "Current Game State:\n";
	for (const auto& player : players) {
		int x, y;
		player.getPosition(x, y);
		std::cout << "Player " << player.getName()
			<< " is at position (" << x << ", " << y << ")\n";
	}
}

void GameSession::playerHits(Player& shooter, Player& target) {
	target.takeHit();
	shooter.addScoreForHit();
}

void GameSession::nextTurn() {
	do {
		currentTurn = (currentTurn + 1) % players.size();
	} while (isCurrentPlayerEliminated());  // Daca jucatorul curent este eliminat, trecem la urmatorul
}

Player& GameSession::getCurrentPlayer() {
	return players[currentTurn];
}

bool GameSession::isCurrentPlayerEliminated() {
	return getCurrentPlayer().getStatus() == PlayerStatus::ELIMINATED;
}

void GameSession::startTurn() {
	std::cout << "\n--- Starting a new turn ---\n";
	std::cout << "It is now " << getCurrentPlayer().getName() << "'s turn\n";
}

// Actualizeaza pozitia unui jucator specificat prin ID
// Aceasta functie cauta jucatorul dupa ID si ii actualizeaza pozitia pe harta la noile coordonate specificate.
// Daca jucatorul este gasit, pozitia este actualizata si se afiseaza un mesaj de confirmare.
// Daca jucatorul nu este gasit, se afiseaza un mesaj de eroare.
void GameSession::updatePlayerPosition(int playerId, int newX, int newY) {
	for (auto& player : players) {
		if (player.getId() == playerId) {
			player.setPosition(newX, newY);
			std::cout << "Player " << player.getName() << " updated to position (" << newX << ", " << newY << ").\n";
			return;
		}
	}
	std::cout << "Player with ID " << playerId << " not found.\n";
}

// Returneaza o lista cu toti jucatorii conectati
// Aceasta functie returneaza vectorul de jucatori din sesiunea curenta.
// Clientii pot folosi aceasta lista pentru a obtine informatii despre toti jucatorii conectati si pozitiile lor.
std::vector<Player> GameSession::GetAllPlayers() const {
	std::cout << "Retrieving all players:\n";
	for (const auto& player : players) {
		int x, y;
		player.getPosition(x, y);
		std::cout << "Player " << player.getName() << " is at position (" << x << ", " << y << ")\n";
	}
	return players;
}

// Elimina un jucator specificat prin ID din sesiune
// Aceasta functie cauta jucatorul dupa ID si il elimina din vectorul de jucatori daca este gasit.
// Daca jucatorul este gasit, se afiseaza un mesaj de confirmare si este eliminat din sesiune.
// Daca jucatorul nu este gasit, se afiseaza un mesaj de eroare.
void GameSession::removePlayerById(int playerId) {
	for (auto it = players.begin(); it != players.end(); ++it) {
		if (it->getId() == playerId) {
			std::cout << "Player " << it->getName() << " with ID " << playerId << " has been removed from the session.\n";
			players.erase(it);
			return;
		}
	}
	std::cout << "Player with ID " << playerId << " not found.\n";
}

void GameSession::recordEvent(std::unique_ptr<Event> event) {
	events.push_back(std::move(event));
}

void GameSession::processEvents() const {
	for (const auto& event : events) {
		std::cout << event->getDescription() << std::endl;
	}
}
//Metoda afisare clasament
void GameSession::displayLeaderboard() const {
	std::vector<Player> sortedPlayers = players;

	// Sorteaza jucatorii dupa scor descrescator
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& a, const Player& b) {
		return a.getScore() > b.getScore();
		});

	// Afiseaza clasamentul
	std::cout << "Leaderboard:" << std::endl;
	for (size_t i = 0; i < sortedPlayers.size(); ++i) {
		const auto& player = sortedPlayers[i];
		std::cout << i + 1 << ". " << player.getName() << ": " << player.getScore() << " points" << std::endl;
	}
