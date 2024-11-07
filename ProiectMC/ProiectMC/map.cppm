export module map;

import <vector>;
import <iostream>;
import <random>;

export enum class CellType { EMPTY, DESTRUCTIBLE_WALL, INDESTRUCTIBLE_WALL };

export class Map {
private:
    std::vector<std::vector<CellType>> grid;  // Matrix for the map
    int width, height;

public:
    // Constructor that sets dimensions and initializes the map with empty spaces
    Map(int n, int m);

    // Returns the type of cell at coordinates (x, y)
    CellType getCellType(int x, int y) const;

    // Sets the cell type at coordinates (x, y)
    void setCellType(int x, int y, CellType type);

    // Generates a map with destructible and indestructible walls
    void generateRandomMap();

    // Displays the map in the console for verification
    void displayMap() const;
};