#pragma once
#include<vector>

enum class CellType { EMPTY, WALL };

class Map
{
private:
	std::vector<std::vector<CellType>> grid;
public:
	Map(int n, int m) : grid(n, std::vector<CellType>(m, CellType::EMPTY)) {}

	CellType getCellType(int x, int y) const
	{
		return grid[x][y];
	}


};

