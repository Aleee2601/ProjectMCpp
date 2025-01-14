#include <sqlite_orm/sqlite_orm.h>
#include "../include/DBPlayer.h"
#include <iostream>


void test() {
	std::cout << "Test";
};
//// Functie pentru initializarea conexiunii la baza de date
//auto initStorage() {
//    using namespace sqlite_orm;
//    // Ne conectam la baza de date
//    return make_storage("BattleCity.sqlite");
//}
//
//// Functie pentru inserarea unui player
//void insertPlayer(const DBPlayer& player) {
//    try {
//        // Initializeaza conexiunea la baza de date
//        auto storage = initStorage();
//
//        // Insereaza player-ul in tabela existenta
//        storage.insert(player);
//        std::cout << "Player inserted: " << player.nickname << "\n";
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error inserting player: " << e.what() << "\n";
//    }
//}