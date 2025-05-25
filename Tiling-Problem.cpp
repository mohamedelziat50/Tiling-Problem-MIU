// Access our Tiling Problem all files/codes on GitHub (if needed): https://github.com/mohamedelziat50/Tiling-Problem-MIU
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

/*
    -----------------------------------------------------
    VISUALIZATION:
    -----------------------------------------------------
    For a step-by-step visual representation of the brute force algorithm,
    open the file: "brute-force-visualization.html" in your browser.
    
    The HTML file provides an interactive visualization that shows:
    - How dominoes are placed and backtracked
    - Real-time step navigation
    - Autoplay functionality with speed control
    - Visual representation of the 2xN grid tiling process (manual or automatic)
    -----------------------------------------------------
*/

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
    cout << "========================================" << endl;
    cout << "         2xN DOMINO TILING         " << endl;
    cout << "========================================" << endl;
    cout << endl;

    // ----------- FIRST APPROACH: Brute Force via Backtracking ------------
    
    cout << "METHOD 1: BRUTE FORCE BACKTRACKING" << endl;
    cout << "---------------------------------------" << endl;
    
    int n;
    cout << "Enter number of columns for 2xn grid: ";
    cin >> n;
    cout << endl;

    // Initialize the 2 x n grid with all cells initially empty (false)
    vector<vector<bool>> grid(ROWS, vector<bool>(n, false));
    
    // Call the recursive function to count valid tilings
    placeDomino(0, 0, grid);

    cout << "   RESULTS:" << endl;
    cout << "   Grid Size: 2 x " << n << endl;
    cout << "   Dominoes Needed: " << (2 * n) / 2 << endl;
    cout << "   Total Valid Tilings: " << tilingCount << endl;
    cout << endl;

    // Reset counter for next method
    tilingCount = 0;

    cout << "===============================================" << endl;
    cout << endl;

    // ----------- SECOND APPROACH: Divide and Conquer ------------

    cout << "METHOD 2: DIVIDE AND CONQUER" << endl;
    cout << "--------------------------------" << endl;
    
    int m; // Use a different variable name to avoid redeclaration error
    cout << "Enter number of columns for 2xn grid: ";
    cin >> m;
    cout << endl;

    // Call the divide and conquer recursive function to get tiling count
    int result = divideAndConquerTiling(m);

    cout << "   RESULTS:" << endl;
    cout << "   Grid Size: 2 x " << m << endl;
    cout << "   Dominoes Needed: " << (2 * m) / 2 << endl;
    cout << "   Total Valid Tilings: " << result << endl;
    cout << endl;

    cout << "===============================================" << endl;
    cout << "           CALCULATION COMPLETE!           " << endl;
    cout << "===============================================" << endl;

    return 0;
}

