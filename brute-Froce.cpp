#include <iostream>
#include <vector>
using namespace std;

int tilingCount = 0;

// Backtracking function with tile tracking
void placeDomino(int row, int col, vector<vector<bool>>& grid) {
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
        return;
    }

    // Try horizontal placement
    if (col + 1 < cols && !grid[row][col + 1]) {
        grid[row][col] = grid[row][col + 1] = true;
        placeDomino(row, col, grid);
        grid[row][col] = grid[row][col + 1] = false;
    }

    // Try vertical placement
    if (row + 1 < rows && !grid[row + 1][col]) {
        grid[row][col] = grid[row + 1][col] = true;
        placeDomino(row, col, grid);
        grid[row][col] = grid[row + 1][col] = false;
    }
}

int main() {
    int n;
    cout << "Enter columns: ";
    cin >> n;

    vector<vector<bool>> grid(2, vector<bool>(n, false));

    placeDomino(0, 0, grid);

    cout << "Number of tiles needed: " << (2 * n) / 2 << endl;
    cout << "Possible tilings: " << tilingCount << endl;
}
