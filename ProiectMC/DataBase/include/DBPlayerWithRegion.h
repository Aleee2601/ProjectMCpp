#pragma once
#include "DBPlayer.h"
class DBPlayerWithRegion :public DBPlayer
{
public:
	int region_id;
	//Constructor
	DBPlayerWithRegion();
	DBPlayerWithRegion(DBPlayer dbplayer, int region_id);
};

