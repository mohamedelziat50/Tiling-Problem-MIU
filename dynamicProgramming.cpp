#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int M, N; // M rows, N columns
unordered_map<long long, long long> memo;

// Forward declaration
long long dp(int col, int mask);

// Helper function to fill a column recursively
// Parameters:
// - row: current row being processed
// - mask: bitmask representing cells already occupied in current column
// - currMask: bitmask representing cells that will be occupied in next column
// - col: current column index
long long fillColumn(int row, int mask, int currMask, int col) {
    // Base case: finished processing all rows in current column
    if (row == M) {
        return dp(col + 1, currMask);
    }
    
    if (mask & (1 << row)) {
        // Cell already filled by horizontal domino from previous column
        return fillColumn(row + 1, mask, currMask, col);
    } else {
        long long ways = 0;
        
        // Option 1: Place vertical domino (occupies current and next row)
        if (row + 1 < M && !(mask & (1 << (row + 1)))) {
            ways += fillColumn(row + 2, mask, currMask, col);
        }
        
        // Option 2: Place horizontal domino (extends to next column)
        ways += fillColumn(row + 1, mask, currMask | (1 << row), col);
        
        return ways;
    }
}

// Recursive DP with memoization
// Parameters:
// - col: current column being processed
// - mask: bitmask representing which cells in current column are already occupied
long long dp(int col, int mask) {
    // Base case: processed all columns, check if grid is completely filled
    if (col == N) return mask == 0; // Must be fully empty to be valid
    
    // Create unique key for memoization
    long long key = ((long long)col << M) | mask;
    if (memo.count(key)) return memo[key];

    // Try to fill column col with current mask
    long long ways = fillColumn(0, mask, 0, col);
    
    memo[key] = ways;
    return ways;
}

int main()
{
    cout << "Enter number of rows (M ≤ 6) and columns (N): ";
    cin >> M >> N;

    if ((M * N) % 2 != 0) {
        cout << "Impossible: odd number of cells!\n";
        return 0;
    }

    long long totalWays = dp(0, 0);
    cout << "Number of tiles needed: " << (M * N) / 2 << endl;
    cout << "Possible tilings: " << totalWays << endl;
    return 0;
}