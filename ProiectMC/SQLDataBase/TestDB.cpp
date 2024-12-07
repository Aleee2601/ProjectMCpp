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