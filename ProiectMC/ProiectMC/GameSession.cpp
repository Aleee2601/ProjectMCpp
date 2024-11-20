#include "GameSession.h"
#include "Player.h"
void GameSession::displayGameState() const {
	std::cout << "Current Game State:\n";
	for (const auto& player : players) {
		int x, y;
		player.getPosition(x, y);
		std::cout << "Player" << player.getName()
			<< "is at position (" << x << "," << y << ")\n";
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

Player& GameSession::getCurentPlayer() {
	return players[currentTurn];
}

bool GameSession::isCurrentPlayerEliminated() {
	return getCurentPlayer().getStatus() == PlayerStatus::ELIMINATED;
}

void GameSession::startTurn() {
	std::cout << "\n--- Starting a new turn ---\n";
	std::cout << "It is now " << getCurentPlayer().getName()<<"turn\n";
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
