#include "GameSession.h"

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