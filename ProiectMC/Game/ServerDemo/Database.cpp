//// Database.cpp
//#include "Database.h"
//
//using namespace sqlite_orm;
//
//Database::Database(const std::string& dbFile)
//    : storage(
//        make_storage(
//            dbFile,
//            make_table("users",
//                make_column("id", &User::id, autoincrement(), primary_key()),
//                make_column("username", &User::username, unique()),
//                make_column("points", &User::points),
//                make_column("score", &User::score)
//            )
//        )
//    )
//{
//    initTables();
//}
//
//void Database::initTables() {
//    storage.sync_schema(); // creează tabele dacă nu există
//}
//
//bool Database::registerUser(const std::string& username) {
//    if (userExists(username)) {
//        return false;
//    }
//    User u{ -1, username, 0, 0 };
//    storage.insert(u);
//    return true;
//}
//
//bool Database::userExists(const std::string& username) {
//    auto count = storage.count<User>(where(c(&User::username) == username));
//    return (count > 0);
//}
