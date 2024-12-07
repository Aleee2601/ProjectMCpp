#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include "DBPlayer.h"
#include "PlayerDAO.h"



static void testInsert(){
	DBPlayer player = PlayerDAO().insertPlayer("maria", "brasov");
	if (player == nullptr)
		std::cout << std::endl << " este null, nickname deja existent " << std::endl;
	else
		std::cout << std::endl << " NU este null " << player.player_id << std::endl;
}

int main() {

    /*
    DBPlayer player = PlayerDAO().insertPlayer("maria","brasov");
    if (player == nullptr)
        std::cout << std::endl << " este null, nickname deja existent " << std::endl;
    else
        std::cout << std::endl << " NU este null " << player.player_id << std::endl;

        */
        /*
        DBPlayer player = PlayerDAO().findPlayerById(37);
        std::cout << std::endl << " " << player.player_id << " " << player.nickname;

        player.score = 8;
        player.points =200;
        PlayerDAO().updatePlayer(player);
       */


    DBPlayer playerLogin = PlayerDAO().loginPlayer("maria", "brasov");
    std::cout << std::endl << " login " << playerLogin.player_id << " " << playerLogin.nickname << " " << playerLogin.password;
    /*

      playerLogin = PlayerDAO().findPlayerByNickname("bombuka nou");
     std::cout << std::endl<< " " << playerLogin.player_id << " " << playerLogin.nickname;
     if (playerLogin == nullptr)
         std::cout << std::endl << " este null " << std::endl;
     else
         std::cout << std::endl << " NU este null "<< std::endl;


     playerLogin = PlayerDAO().findPlayerById(23);
     std::cout << std::endl << " " << playerLogin.player_id << " " << playerLogin.nickname;
  */
    return 0;
}