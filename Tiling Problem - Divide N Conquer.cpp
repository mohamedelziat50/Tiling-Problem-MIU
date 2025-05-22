#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Domino { //Holds coordinates of a placed domino
    int r1, c1, r2, c2; //Represents how the domino is placed (Vertically or Horizontally)
};

int N, M; //Global grid rows & columns
long long totalTilings = 0;
vector<vector<bool>> grid; //2D boolean grid


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

// Recursive backtracking to place dominoes
void Solve(int index, vector<Domino>& placed) {
    //BASE CASE
    if (index == N * M) { //Once index has moved past the last cell (N*M) we have a complete grid tiling
        ++totalTilings;
        PrintTiling(placed);
        return;
    }

    //Turning linear index into 2D coordinates
    int r = index / M;  //How many rows we've gone down
    int c = index % M; //how far into the row we are

    /*
    ~~EXAMPLE~~
[0][1][2][3][4]
[5][6][7][8][9]
[10][11][12][13][14]

-> M = 5 (5 columns)
For index=13 , 13 / 5 = 2 , that means we're in third row (2nd element).
For index=13 , 13 % 5 = 3 , that means we're in fourth column (3rd element).
    */

    if (grid[r][c]) { //if cell is already covered (== true) skip to the next cell
        Solve(index + 1, placed);
        return;
    }

    // Try horizontal placement
    if (c + 1 < M && !grid[r][c + 1]) { //Ensuring we're not in the rightmost column & the cell to the right is free
        grid[r][c] = grid[r][c + 1] = true; //mark both cells (horizontally) as covered
        placed.push_back({ r, c, r, c + 1 }); //pushback a domino with its cell placement credentials into "placed" vector
        Solve(index + 1, placed); //recurse with next index to continue attempting to fill the rest of the grid horizontally
       
        placed.pop_back(); //Once the recursive call returns, we need to undo (backtrack) to try other possibilities
        grid[r][c] = grid[r][c + 1] = false;

    /* The above two lines are there to ensure we can check other filling options to find the total number of filling possibilities
   " Okay, I just tried placing a domino here and explored everything that came after it. Now let’s reset back to exactly what
    the board looked like before I placed it, so I can try the next option. "
    */
    }

    // Try vertical placement
    if (r + 1 < N && !grid[r + 1][c]) { //same pattern but places downwards into (r+1,c)
        grid[r][c] = grid[r + 1][c] = true;
        placed.push_back({ r, c, r + 1, c });
        Solve(index + 1, placed);
        placed.pop_back();
        grid[r][c] = grid[r + 1][c] = false;
    }
}

int main() {
    cout << "Enter number of rows (N) and columns (M): ";
    if (!(cin >> N >> M)) { //if inputs are not integer
        cerr << "Invalid input. Please enter two integers.\n";
    }
    if ((N * M) % 2 != 0) { //if number of cells aren't even
        cerr << "Error: N * M must be even to tile with dominoes.\n";
    }

    //intializes grid to an NxM 2D vector of false values
    grid.assign(N, vector<bool>(M, false));
    vector<Domino> placed; //Vector to track placed dominoes

    Solve(0, placed); //start attempting to fill out the grid from first index

    cout << "Total number of valid tilings: " << totalTilings << "\n";
    cout << "Each tiling uses " << (N * M) / 2 << " dominoes.\n";
    return 0;
}
