#include <iostream>
#include <vector>
using namespace std;

int tilingCount = 0;

// Function to print the grid using tile IDs
void printGrid(const vector<vector<char>>& visualGrid) {
    for (const auto& row : visualGrid) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << '\n';
    }
    cout << "------------------\n";
}

// Backtracking function with tile tracking
void placeDomino(int row, int col, vector<vector<bool>>& grid,
    vector<vector<char>>& visualGrid, char tileID) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Skip to next empty cell
    while (row < rows && grid[row][col]) {
        col++;
        if (col == cols) {
            col = 0;
            row++;
        }
    }

    // Base case: full coverage
    if (row == rows) {
        tilingCount++;
        cout << "Tiling #" << tilingCount << ":\n";
        printGrid(visualGrid);
        return;
    }

    // Try horizontal placement
    if (col + 1 < cols && !grid[row][col + 1]) {
        grid[row][col] = grid[row][col + 1] = true;
        visualGrid[row][col] = visualGrid[row][col + 1] = tileID;
        placeDomino(row, col, grid, visualGrid, tileID + 1);
        grid[row][col] = grid[row][col + 1] = false;
        visualGrid[row][col] = visualGrid[row][col + 1] = '.';
    }

    // Try vertical placement
    if (row + 1 < rows && !grid[row + 1][col]) {
        grid[row][col] = grid[row + 1][col] = true;
        visualGrid[row][col] = visualGrid[row + 1][col] = tileID;
        placeDomino(row, col, grid, visualGrid, tileID + 1);
        grid[row][col] = grid[row + 1][col] = false;
        visualGrid[row][col] = visualGrid[row + 1][col] = '.';
    }
}

int main() {
    int m, n;
    cout << "Enter rows and columns: ";
    cin >> m >> n;

    if (m * n % 2 != 0) {
        cout << "Impossible: odd number of cells!\n";
        return 0;
    }

    vector<vector<bool>> grid(m, vector<bool>(n, false));
    vector<vector<char>> visualGrid(m, vector<char>(n, '.'));

    placeDomino(0, 0, grid, visualGrid, 'A');

    cout << "Number of tiles needed: " << (m * n) / 2 << endl;
    cout << "Possible tilings: " << tilingCount << endl;
}
