#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Domino {
    int r1, c1, r2, c2;
};

int N, M;
long long totalTilings = 0;
vector<vector<bool>> grid;

// This function uses traditional Divide & Conquer
// For a 2xN grid, number of ways to tile is:
//   F(n) = F(n-1) + F(n-2)
// Like the Fibonacci sequence!
long long countTilings(int n, vector<long long>& memo) {
    if (n == 0) return 1;  // Empty grid
    if (n == 1) return 1;  // Only vertical dominoes
    if (memo[n] != -1) return memo[n];

    // Divide: 2x(n-1) and 2x(n-2)
    memo[n] = countTilings(n - 1, memo) + countTilings(n - 2, memo);

    return memo[n];
}

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

int main() {
    cout << "Enter number of rows (N) and columns (M): ";
    if (!(cin >> N >> M)) { //if inputs are not integer
        cerr << "Invalid input. Please enter two integers.\n";
    }
    if ((N * M) % 2 != 0) { //if number of cells aren't even
        cerr << "Error: N * M must be even to tile with dominoes.\n";
    }

    grid.assign(N, vector<bool>(M, false));

    long long ways = countTilings(length, grid);
    cout << "Number of ways to tile the grid: " << ways << "\n";
    cout << "Each tiling uses " << (N * M) / 2 << " dominoes.\n";

    return 0;
}
