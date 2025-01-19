#include "../../include/db/DBPlayer.h"
#include "../../include/db/PlayerDAO.h"
#include "SQLLiteDataBase.cpp"
#include <sqlite_orm/sqlite_orm.h>
using namespace sqlite_orm;

DBPlayer PlayerDAO::findPlayerById(int id)
{
    DBPlayer player = nullptr;
    auto storage = initDatabase();
    try {
        player = storage.get<DBPlayer>(id);
    }
    catch (...) {
        std::cout << "player not found id:" << id << std::endl;
    }
    return player;
}

DBPlayer PlayerDAO::insertPlayer(std::string nickname, std::string password)
{
    DBPlayer player;
    player.nickname = nickname;
    player.password = password;
    auto storage = initDatabase();

    // nickname trebuie sa fie unic !
    if (findPlayerByNickname(nickname) == nullptr) {
        // se recupereaza id_ul dupa inserare
        player.player_id = storage.insert(player);
        std::cout << "player inserted, id: " << player.player_id << std::endl;
        return player;
    }
    else {
        std::cout << "nickname already exists ! " << player.nickname << std::endl;
        return nullptr;
    }

}
//
//bool PlayerDAO::insertPlayer(std::string nickname, std::string password)
//{
//    DBPlayer player;
//    player.nickname = nickname;
//    player.password = password;
//    auto storage = initDatabase();
//
//    // nickname trebuie sa fie unic
//    if (findPlayerByNickname(nickname) == nullptr) {
//        // se recuperează id-ul după inserare
//        player.player_id = storage.insert(player);
//        std::cout << "player inserted, id: " << player.player_id << std::endl;
//        return true;  // inserarea a avut succes
//    }
//    else {
//        std::cout << "nickname already exists ! " << player.nickname << std::endl;
//        return false;  // nickname-ul există deja
//    }
//}



DBPlayer PlayerDAO::findPlayerByNickname(std::string nickname)
{

    auto storage = initDatabase();
    //  auto players = storage.get_all<DBPlayer>();
    //  for (const auto& onePlayer : players)
    //      if (onePlayer.nickname == nickname)
    //          return onePlayer;
    //return nullptr;

    auto players = storage.get_all<DBPlayer>(
        where(
            is_equal(&DBPlayer::nickname, nickname)
            )
    );
    if (players.size() > 0)
        return players[0];
    return nullptr;

}

void PlayerDAO::updatePlayer(DBPlayer& player)
{
    auto storage = initDatabase();;
    storage.update(player);
    std::cout << "player updated" << std::endl;
}



DBPlayer PlayerDAO::loginPlayer(std::string nickname, std::string password)
{
    auto storage = initDatabase();
    //auto players = storage.get_all<DBPlayer>();
    //for (const auto& onePlayer : players) 
    //    if (onePlayer.nickname == nickname && onePlayer.password == password) {
    //        return onePlayer;
    //    }
    // return nullptr;

    auto players = storage.get_all<DBPlayer>(
        where(
            and_(
                is_equal(&DBPlayer::nickname, nickname),
                is_equal(&DBPlayer::password, password)
            )
            )
    );
    if (players.size() > 0)
        return players[0];
    return nullptr;

}

void PlayerDAO::updatePlayerScore(int idPlayer, int score)
{
    auto storage = initDatabase();
    DBPlayer player = PlayerDAO().findPlayerById(idPlayer);
    player.score = player.score + score;
    storage.update(player);
    std::cout << "score player updated" << std::endl;
}