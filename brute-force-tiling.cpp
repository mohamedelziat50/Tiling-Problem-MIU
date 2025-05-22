#include <iostream>
#include <vector>
using namespace std;

int tilingCount = 0;

// Essential backtracking components:
// 1. Track current state (grid)
// 2. Find next empty cell
// 3. Try valid placements
// 4. Recurse/backtrack
void placeDomino(int row, int col, vector<vector<bool>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    
    // Find first empty cell
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
    if (col+1 < cols && !grid[row][col+1]) {
        grid[row][col] = grid[row][col+1] = true;
        placeDomino(row, col, grid);
        grid[row][col] = grid[row][col+1] = false;
    }

    // Try vertical placement
    if (row+1 < rows && !grid[row+1][col]) {
        grid[row][col] = grid[row+1][col] = true;
        placeDomino(row, col, grid);
        grid[row][col] = grid[row+1][col] = false;
    }
}

int main() {
    int m, n;
    cout << "Enter rows and columns: ";
    cin >> m >> n;
    
    if (m*n % 2 != 0) {
        cout << "Impossible: odd number of cells!\n";
        return 0;
    }

    vector<vector<bool>> grid(m, vector<bool>(n, false));
    placeDomino(0, 0, grid);
    
    cout << "Number of tiles needed: " << (m * n) / 2 << endl;
    cout << "Possible tilings: " << tilingCount << endl;
}