#include "../../include/db/DBPlayerWithRegion.h"
#include "../../include/db/DBPlayerWithRegion.h"

// Constructor 
DBPlayerWithRegion::DBPlayerWithRegion() {}

DBPlayerWithRegion::DBPlayerWithRegion(DBPlayer dbplayer, int region_id) :DBPlayer(dbplayer), region_id(region_id) {

};

