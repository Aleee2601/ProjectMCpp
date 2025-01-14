#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include "../../include/db/DBPlayer.h"
#include "../../include/db/PlayerDAO.h"
#include "../../include/db/GameDAO.h"
#include "../../include/db/DBPlayerWithRegion.h"

#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

static void testInsert() {
    DBPlayer player = PlayerDAO().insertPlayer("maria", "brasov");
    if (player == nullptr)
        std::cout << std::endl << " este null, nickname deja existent " << std::endl;
    else
        std::cout << std::endl << " NU este null " << player.player_id << std::endl;
}
 /*
int main() {


    DBPlayer player = PlayerDAO().insertPlayer("maria", "brasov");
    if (player == nullptr)
        std::cout << std::endl << " este null, nickname deja existent " << std::endl;
    else
        std::cout << std::endl << " NU este null " << player.player_id << std::endl;



    int idPlayer = 1;
    int score = 200;
    PlayerDAO().updatePlayerScore(idPlayer, score);

   
    DBPlayer player = PlayerDAO().findPlayerById(37);
    std::cout << std::endl << " " << player.player_id << " " << player.nickname;

    player.score = 8;
    player.points =200;
    PlayerDAO().updatePlayer(player);
   */


   //   DBPlayer playerLogin = PlayerDAO().loginPlayer("maria", "brasov");
   //   std::cout << std::endl << " login " << playerLogin.player_id << " " << playerLogin.nickname << " " << playerLogin.password;
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


    // Teste pentru Creare Game / End Game

      /*
      DBPlayer player1 = PlayerDAO().findPlayerById(1);
      DBPlayer player2 = PlayerDAO().findPlayerById(2);
      DBPlayer player3 = PlayerDAO().findPlayerById(3);
      DBPlayer player4 = PlayerDAO().findPlayerById(4);

      DBPlayerWithRegion playerA = DBPlayerWithRegion(player1, 1);
      DBPlayerWithRegion playerB = DBPlayerWithRegion(player2, 1);
      DBPlayerWithRegion playerC = DBPlayerWithRegion(player3, 2);
      DBPlayerWithRegion playerD = DBPlayerWithRegion(player4, 2);

      std::vector<DBPlayerWithRegion> players(4);
      players[0] = playerA;
      players[1] = playerB;
      players[2] = playerC;
      players[3] = playerD;

      DBGame game = GameDAO().createGame(players);
      std::cout << std::endl << "Am creat un joc cu Id Game: " << game.game_id;


      game.game_region_win_id = 2;
      players[0].points = 100;
      players[1].points = 200;
      players[2].points = 300;
      players[3].points = 400;

      std::cout << std::endl << "Finalul jocului - salvam datele - Id Game:" << game.game_id;
      GameDAO().updateEndedGame(game, players);
     



    return 0;
} */
