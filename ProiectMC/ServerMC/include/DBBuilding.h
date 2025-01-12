#pragma once
#include <string>
#include <iostream>

class DBBuilding {
public:
    int building_id;
    int buildingRegion_id;
    std::string building_name;

    DBBuilding();
    DBBuilding(int id, int region_id, const std::string& name);
    ~DBBuilding();

    void setBuilding(int id, int region_id, const std::string& name);
    int getBuildingId() const;
    std::string getBuildingName() const;
    int getBuildingRegionId() const;


};
