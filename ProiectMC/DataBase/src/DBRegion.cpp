#include "../include/DBRegion.h"

DBRegion::DBRegion() : region_id(0), region_name("") {}

DBRegion::DBRegion(int id, const std::string& name)
    : region_id(id), region_name(name) {}

void DBRegion::setRegion(int id, const std::string& name) {
    region_id = id;
    region_name = name;
}

int DBRegion::getRegionId() const {
    return region_id;
}

std::string DBRegion::getRegionName() const {
    return region_name;
}
