#pragma once
#include <string>
#include <iostream>

class DBRegion
{
public:
    int region_id;
    std::string region_name;

    
    DBRegion();
    DBRegion(int id, const std::string& name);

    
    void setRegion(int id, const std::string& name);
    int getRegionId() const;
    std::string getRegionName() const;
};
