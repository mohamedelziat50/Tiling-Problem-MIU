#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <functional>

using namespace std;

struct Domino { //Holds coordinates of a placed domino
    int r1, c1, r2, c2; //Represents how the domino is placed (Vertically or Horizontally)
};

int N, M; //Global grid rows & columns
long long totalTilings = 0;
vector<vector<bool>> grid; //2D boolean grid

// For divide and conquer approach
map<pair<pair<int,int>, pair<int,int>>, vector<vector<Domino>>> memo;


/* vector<vector<bool>> grid EXPLAINED
 
[] -> { [] , [] , [] , [] } 
[] -> { [] , [] , [] , [] }
[] -> { [] , [] , [] , [] }
[] -> { [] , [] , [] , [] }
^ This is N ( number of rows) that points to a vector of M columns
*/

// Print a single tiling using ASCII art with domino IDs
void PrintTiling(const vector<Domino>& placed) {

    vector<vector<int>> display(N, vector<int>(M, 0)); //Empty 2D grid to fill for each cout representation

    for (int i = 0; i < placed.size(); ++i) {
        //retrive the domino as d and store id in both its coordinate cells in display
        const auto& d = placed[i]; //fetches first placed domino
        int id = i + 1; //id starts from 1 for human eye convenience when printing
        display[d.r1][d.c1] = id;
        display[d.r2][d.c2] = id;
    }

    cout << "Tiling #" << totalTilings << " (" << (N * M) / 2 << " dominoes):\n";
    // Print column headers
    cout << "    ";
    for (int c = 0; c < M; ++c) cout << setw(3) << c + 1;
    cout << "\n";
    cout << "   +" << string(M * 3, '-') << "+\n";
    for (int r = 0; r < N; ++r) {
        cout << setw(2) << r + 1 << " |";
        for (int c = 0; c < M; ++c) {
            if (display[r][c] > 0)
                cout << setw(3) << display[r][c];
            else
                cout << setw(3) << '.';
        }
        cout << " |\n";
    }
    cout << "   +" << string(M * 3, '-') << "+\n\n";
}

// Check if a region has any cells already occupied
bool hasOccupiedCells(int startR, int endR, int startC, int endC) {
    for (int r = startR; r < endR; r++) {
        for (int c = startC; c < endC; c++) {
            if (grid[r][c]) return true;
        }
    }
    return false;
}

// Generate all possible ways to tile the boundary between two regions
vector<vector<Domino>> generateBoundaryTilings(int startR, int endR, int startC, int endC, int splitR, int splitC, bool isVerticalSplit) {
    vector<vector<Domino>> boundaryTilings;
    
    if (isVerticalSplit) {
        // Split vertically at column splitC
        // Need to handle dominoes that cross the boundary from left region to right region
        int rows = endR - startR;
        
        // Generate all ways to place horizontal dominoes across the boundary
        function<void(int, vector<Domino>&, vector<bool>&)> generateCrossings = 
            [&](int row, vector<Domino>& current, vector<bool>& used) {
                if (row >= rows) {
                    boundaryTilings.push_back(current);
                    return;
                }
                
                if (used[row]) {
                    generateCrossings(row + 1, current, used);
                    return;
                }
                
                // Option 1: Place horizontal domino across boundary
                current.push_back({startR + row, splitC - 1, startR + row, splitC});
                used[row] = true;
                generateCrossings(row + 1, current, used);
                used[row] = false;
                current.pop_back();
                
                // Option 2: Skip this row (it will be handled by internal tilings)
                generateCrossings(row + 1, current, used);
            };
        
        vector<Domino> current;
        vector<bool> used(rows, false);
        generateCrossings(0, current, used);
        
    } else {
        // Split horizontally at row splitR
        // Need to handle dominoes that cross the boundary from top region to bottom region
        int cols = endC - startC;
        
        // Generate all ways to place vertical dominoes across the boundary
        function<void(int, vector<Domino>&, vector<bool>&)> generateCrossings = 
            [&](int col, vector<Domino>& current, vector<bool>& used) {
                if (col >= cols) {
                    boundaryTilings.push_back(current);
                    return;
                }
                
                if (used[col]) {
                    generateCrossings(col + 1, current, used);
                    return;
                }
                
                // Option 1: Place vertical domino across boundary
                current.push_back({splitR - 1, startC + col, splitR, startC + col});
                used[col] = true;
                generateCrossings(col + 1, current, used);
                used[col] = false;
                current.pop_back();
                
                // Option 2: Skip this column (it will be handled by internal tilings)
                generateCrossings(col + 1, current, used);
            };
        
        vector<Domino> current;
        vector<bool> used(cols, false);
        generateCrossings(0, current, used);
    }
    
    return boundaryTilings;
}

// Divide and Conquer approach to solve tiling
vector<vector<Domino>> divideAndConquer(int startR, int endR, int startC, int endC) {
    vector<vector<Domino>> solutions;
    
    // Base case: small regions that can be solved directly
    int rows = endR - startR;
    int cols = endC - startC;
    
    if (rows * cols <= 4) {
        // Solve small region with backtracking
        vector<Domino> currentSolution;
        vector<vector<bool>> localGrid(rows, vector<bool>(cols, false));
        
        function<void(int)> backtrack = [&](int pos) {
            if (pos >= rows * cols) {
                solutions.push_back(currentSolution);
                return;
            }
            
            int r = pos / cols;
            int c = pos % cols;
            
            if (localGrid[r][c]) {
                backtrack(pos + 1);
                return;
            }
            
            // Try horizontal
            if (c + 1 < cols && !localGrid[r][c + 1]) {
                localGrid[r][c] = localGrid[r][c + 1] = true;
                currentSolution.push_back({startR + r, startC + c, startR + r, startC + c + 1});
                backtrack(pos + 1);
                currentSolution.pop_back();
                localGrid[r][c] = localGrid[r][c + 1] = false;
            }
            
            // Try vertical
            if (r + 1 < rows && !localGrid[r + 1][c]) {
                localGrid[r][c] = localGrid[r + 1][c] = true;
                currentSolution.push_back({startR + r, startC + c, startR + r + 1, startC + c});
                backtrack(pos + 1);
                currentSolution.pop_back();
                localGrid[r][c] = localGrid[r + 1][c] = false;
            }
        };
        
        backtrack(0);
        return solutions;
    }
    
    // Divide: choose split point
    bool splitVertically = (cols > rows);
    int splitR = startR + rows / 2;
    int splitC = startC + cols / 2;
    
    if (splitVertically) {
        // Split vertically
        auto leftSolutions = divideAndConquer(startR, endR, startC, splitC);
        auto rightSolutions = divideAndConquer(startR, endR, splitC, endC);
        auto boundarySolutions = generateBoundaryTilings(startR, endR, startC, endC, splitR, splitC, true);
        
        // Combine solutions
        for (const auto& left : leftSolutions) {
            for (const auto& right : rightSolutions) {
                for (const auto& boundary : boundarySolutions) {
                    vector<Domino> combined = left;
                    combined.insert(combined.end(), right.begin(), right.end());
                    combined.insert(combined.end(), boundary.begin(), boundary.end());
                    solutions.push_back(combined);
                }
            }
        }
    } else {
        // Split horizontally
        auto topSolutions = divideAndConquer(startR, splitR, startC, endC);
        auto bottomSolutions = divideAndConquer(splitR, endR, startC, endC);
        auto boundarySolutions = generateBoundaryTilings(startR, endR, startC, endC, splitR, splitC, false);
        
        // Combine solutions
        for (const auto& top : topSolutions) {
            for (const auto& bottom : bottomSolutions) {
                for (const auto& boundary : boundarySolutions) {
                    vector<Domino> combined = top;
                    combined.insert(combined.end(), bottom.begin(), bottom.end());
                    combined.insert(combined.end(), boundary.begin(), boundary.end());
                    solutions.push_back(combined);
                }
            }
        }
    }
    
    return solutions;
}

// Main solving function using divide and conquer
void SolveDivideConquer() {
    auto allSolutions = divideAndConquer(0, N, 0, M);
    
    for (const auto& solution : allSolutions) {
        ++totalTilings;
        PrintTiling(solution);
    }
}

int main() {
    cout << "Enter number of rows (N) and columns (M): ";
    if (!(cin >> N >> M)) { //if inputs are not integer
        cerr << "Invalid input. Please enter two integers.\n";
        return 1;
    }
    if ((N * M) % 2 != 0) { //if number of cells aren't even
        cerr << "Error: N * M must be even to tile with dominoes.\n";
        return 1;
    }

    //intializes grid to an NxM 2D vector of false values
    grid.assign(N, vector<bool>(M, false));

    SolveDivideConquer(); //Use divide and conquer approach

    cout << "Total number of valid tilings: " << totalTilings << "\n";
    cout << "Each tiling uses " << (N * M) / 2 << " dominoeeeeeees.\n";
    return 0;
}
