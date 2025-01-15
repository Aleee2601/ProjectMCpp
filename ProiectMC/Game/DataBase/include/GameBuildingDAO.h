#pragma once
#include "DBGame.h"
#include <vector>
#include "DBPlayerWithRegion.h"
#include "DBGameBuilding.h"
#include "DBBuilding.h"
#include "DBPlayer.h"


using namespace std;

class GameBuildingDAO
{

public:
	DBGameBuilding insertRecord(int game_id, int building_id, int player_id);
	DBGameBuilding insertRecord(DBGame game, DBBuilding building, DBPlayer player);
};

