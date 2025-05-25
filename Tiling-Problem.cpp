#include <iostream>
#include <vector>

using namespace std;


/*
    -----------------------------------------------------
    Objective:
    -----------------------------------------------------
    Given a 2-row by n-column grid (2 x n), calculate the total number of unique ways it can be
    tiled using 2 x 1 domino pieces.

*/

// Fixed number of rows for the grid (2 x n)
const int ROWS = 2; 
// Global counter to count valid tiling configurations
int tilingCount = 0;

// -------------------------------
// BRUTE FORCE APPROACH --> traverses all possible placements of dominoes using backtracking
// Time Complexity: O(2^n)
// Space Complexity: O(n) 
// -------------------------------

void placeDomino(int row, int col, vector<vector<bool>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Move to next empty cell (skip filled ones)
    while (row < rows && grid[row][col]) {
        col++;
        // Move to next row if column limit reached
        if (col == cols) { 
            col = 0;
            row++;
        }

    }

    // Base case: if we've covered the whole grid
    if (row == rows) {
        // Found one valid way to tile
        tilingCount++; 
        return;
    }

    // Try placing a domino horizontally (occupies two cells in same row)
    if (col + 1 < cols && !grid[row][col + 1]) {

        // Mark cells as filled
        grid[row][col] = grid[row][col + 1] = true; 

        // Recurse to next placement
        placeDomino(row, col, grid);  

        // Backtrack
        grid[row][col] = grid[row][col + 1] = false; 
    }

    // Try placing a domino vertically (occupies two cells in same column)
    if (row + 1 < rows && !grid[row + 1][col]) {

        // Mark cells as filled
        grid[row][col] = grid[row + 1][col] = true;

        // Recurse to next placement
        placeDomino(row, col, grid); 

        // Backtrack
        grid[row][col] = grid[row + 1][col] = false;
    }
}

// -------------------------------
// DIVIDE AND CONQUER APPROACH
// Time Complexity: O(2^n)
// Space Complexity: O(n) due to recursion stack
// -------------------------------

int divideAndConquerTiling(int n) {
    // Base cases:
    // If n = 0, there's 1 way to tile an empty board (do nothing)
    // If n = 1, there's 1 way to place one vertical domino
    if (n == 0) return 1;
    if (n == 1) return 1;

	//using a way really close to Fibonacci sequence
    return divideAndConquerTiling(n - 1) + divideAndConquerTiling(n - 2);
}

int main() {

    // ----------- FIRST APPROACH: Brute Force via Backtracking ------------

    int n;
    cout << "Enter columns for brute force method (2 x n grid): ";
    cin >> n;

    // Initialize the 2 x n grid with all cells initially empty (false)
    vector<vector<bool>> grid(ROWS, vector<bool>(n, false));

    // Call the recursive function to count valid tilings
    placeDomino(0, 0, grid);

    // Output number of tiles (each domino covers 2 squares, so we need (2 * n) / 2 tiles)
    cout << "Number of tiles needed: " << (2 * n) / 2 << endl;

    // Output total number of valid ways to tile the 2 x n grid
    cout << "Possible tilings using brute force: " << tilingCount << endl;



    // ----------- SECOND APPROACH: Divide and Conquer ------------

    int m; // Use a different variable name to avoid redeclaration error
    cout << "Enter columns for divide and conquer method (2 x n grid): ";
    cin >> m;

    // Call the divide and conquer recursive function to get tiling count
    int result = divideAndConquerTiling(m);

    // Output the result from divide and conquer method
    cout << "Number of ways to tile 2 x " << m << " grid using divide and conquer: " << result << endl;

    return 0;

}

