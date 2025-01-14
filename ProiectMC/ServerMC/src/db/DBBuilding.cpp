#include "../../include/db/DBBuilding.h"

DBBuilding::DBBuilding() : building_id(0), buildingRegion_id(0), building_name("") {}

DBBuilding::DBBuilding(int id, int region_id, const std::string& name)
    : building_id(id), buildingRegion_id(region_id), building_name(name) {}

DBBuilding::~DBBuilding() {}

void DBBuilding::setBuilding(int id, int region_id, const std::string& name) {
    building_id = id;
    buildingRegion_id = region_id;
    building_name = name;
}

int DBBuilding::getBuildingId() const {
    return building_id;
}

std::string DBBuilding::getBuildingName() const {
    return building_name;
}

int DBBuilding::getBuildingRegionId() const {
    return buildingRegion_id;
}