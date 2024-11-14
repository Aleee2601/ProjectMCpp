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