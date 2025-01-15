//#pragma once
//// Database.h
//#pragma once
//#include <string>
//#include "sqlite_orm/sqlite_orm.h"
//
//struct User {
//    int id;
//    std::string username;
//    int points;
//    int score;
//};
//
//class Database {
//public:
//    Database(const std::string& dbFile);
//
//    bool registerUser(const std::string& username);
//    bool userExists(const std::string& username);
//
//private:
//    decltype(sqlite_orm::make_storage("")) storage; // tip dedus
//
//    // inițializare a structurilor de date
//    void initTables();
//};
