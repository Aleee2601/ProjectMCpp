//#include <iostream>
//#include "map.h"
//#include "Player.h"
//#include "GameSession.h"
//
//int main() {
//    // Creăm jucători și testăm funcțiile individuale
//    Player player1(1, "Alex", 0, 0);
//    player1.setPosition(5, 10);
//    player1.addScore(150);
//    player1.displayStatus();
//
//    int x, y;
//    player1.getPosition(x, y);
//    std::cout << "Pozitia curenta obtinuta: (" << x << ", " << y << ")" << std::endl;
//    std::cout << "Scorul curent: " << player1.getScore() << std::endl;
//
//    // Creăm o sesiune de joc și adăugăm jucători
//    GameSession session(10, 10);
//    session.addPlayer(player1);
//    Player player2(2, "Bob", 3, 3);
//    session.addPlayer(player2);
//
//    // Testarea funcției de actualizare a poziției unui jucător
//    session.updatePlayerPosition(1, 7, 7); // Actualizează poziția lui Alex
//    session.updatePlayerPosition(2, 4, 4); // Actualizează poziția lui Bob
//
//    // Afișarea stării actuale a jocului
//    std::cout << "\nCurrent Game State:\n";
//    session.displayGameState();
//
//    // Testarea funcției GetAllPlayers
//    std::cout << "\nGetting all players:\n";
//    std::vector<Player> allPlayers = session.getAllPlayers();
//
//    // Testarea funcției removePlayerById
//    std::cout << "\nRemoving player with ID 1:\n";
//    session.removePlayerById(1);
//
//    // Afișarea stării actuale a jocului după eliminarea unui jucător
//    std::cout << "\nCurrent Game State after removing player with ID 1:\n";
//    session.displayGameState();
//
//    // Test 1: Generarea hărții
//    Map map(10, 10);
//    map.displayMap();
//
//    // Contorizare pentru fiecare tip de celulă
//    int emptyCount = 0, destructibleCount = 0, indestructibleCount = 0;
//    for (int i = 0; i < 10; ++i) {
//        for (int j = 0; j < 10; ++j) {
//            CellType type = map.getCellType(i, j);
//            if (type == CellType::EMPTY) emptyCount++;
//            else if (type == CellType::DESTRUCTIBLE_WALL) destructibleCount++;
//            else if (type == CellType::INDESTRUCTIBLE_WALL) indestructibleCount++;
//        }
//    }
//
//    // Print counts to verify diversity in generated map
//    std::cout << "Empty cells: " << emptyCount << "\n";
//    std::cout << "Destructible walls: " << destructibleCount << "\n";
//    std::cout << "Indestructible walls: " << indestructibleCount << "\n";
//
//    // Test 2: Coliziunea cu zidurile
//    int testX = 5, testY = 5;
//    map.setCellType(testX, testY, CellType::DESTRUCTIBLE_WALL);
//    if (map.isCollisionWithWall(testX, testY)) {
//        std::cout << "Collision detected with destructible wall at (" << testX << ", " << testY << ")\n";
//    }
//    else {
//        std::cout << "No collision detected at (" << testX << ", " << testY << ")\n";
//    }
//
//    // Test 3: Distrugerea zidurilor
//    std::cout << "Attempting to destroy wall at (" << testX << ", " << testY << "):\n";
//    map.destroyWallWithDisplay(testX, testY);
//
//    // Test 4: Explozia unei bombe si efectul asupra zidurilor
//    std::cout << "\nTesting bomb Explosion on Walls\n";
//    map.setCellType(4, 4, CellType::DESTRUCTIBLE_WALL);
//    map.setCellType(4, 6, CellType::DESTRUCTIBLE_WALL);
//    Bomb bomb(5, 5);
//    bomb.detonate(map);
//    map.displayMap();
//
//    // Test 5: Explozia unei bombe si efectul asupra jucatorilor
//    std::cout << "\nTesting bomb Explosion on Players\n";
//    std::vector<Player> players = { player1, player2 };
//    player1.setPosition(5, 4);
//    bomb.calculateExplosionEffects(map, players);
//    player1.displayStatus();
//    player2.displayStatus();
//
//    // Test 6: Testare upgrade-uri
//    player1.addScore(50);
//    std::cout << "Player's score after upgrade: " << player1.getScore() << std::endl;
//
//    return 0;
//}
