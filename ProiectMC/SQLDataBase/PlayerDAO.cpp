#include "DBPlayer.h"
#include "PlayerDAO.h"
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