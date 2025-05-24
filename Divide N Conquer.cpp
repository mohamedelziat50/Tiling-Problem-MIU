#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Domino { int r1, c1, r2, c2; };

int N, M;
long long totalTilings = 0;

// Helper to print a solution
void PrintTiling(const vector<Domino>& placed) {
    vector<vector<int>> board(N, vector<int>(M, 0));
    for (int i = 0; i < placed.size(); ++i) {
        const auto& d = placed[i];
        board[d.r1][d.c1] = board[d.r2][d.c2] = i + 1;
    }

    cout << "Tiling #" << totalTilings << ":\n";
    cout << "    ";
    for (int c = 0; c < M; ++c) cout << setw(3) << c + 1;
    cout << "\n   +" << string(M * 3, '-') << "+\n";
    for (int r = 0; r < N; ++r) {
        cout << setw(2) << r + 1 << " |";
        for (int c = 0; c < M; ++c)
            cout << setw(3) << (board[r][c] ? board[r][c] : '.');
        cout << " |\n";
    }
    cout << "   +" << string(M * 3, '-') << "+\n\n";
}

// Check if a full tiling is valid
bool isValidTiling(const vector<Domino>& dominos, int rows, int cols, int r0, int c0) {
    vector<vector<bool>> used(rows, vector<bool>(cols));
    for (auto& d : dominos) {
        int r1 = d.r1 - r0, c1 = d.c1 - c0;
        int r2 = d.r2 - r0, c2 = d.c2 - c0;

        if (min({ r1, r2 }) < 0 || max({ r1, r2 }) >= rows ||
            min({ c1, c2 }) < 0 || max({ c1, c2 }) >= cols)
            return false;

        if (used[r1][c1] || used[r2][c2]) return false;
        used[r1][c1] = used[r2][c2] = true;

        if (!((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1)))
            return false;
    }

    for (auto& row : used)
        for (bool cell : row)
            if (!cell) return false;
    return true;
}

// Generate all tilings of a region, respecting blocked cells
vector<vector<Domino>> tileRegion(int r0, int r1, int c0, int c1, const vector<Domino>& prefilled) {
    int rows = r1 - r0, cols = c1 - c0;
    vector<vector<bool>> blocked(rows, vector<bool>(cols));
    for (auto& d : prefilled) {
        if (d.r1 >= r0 && d.r1 < r1 && d.c1 >= c0 && d.c1 < c1)
            blocked[d.r1 - r0][d.c1 - c0] = true;
        if (d.r2 >= r0 && d.r2 < r1 && d.c2 >= c0 && d.c2 < c1)
            blocked[d.r2 - r0][d.c2 - c0] = true;
    }

    vector<vector<Domino>> res;
    vector<Domino> curr;

    function<void(int)> bt = [&](int idx) {
        if (idx == rows * cols) {
            res.push_back(curr);
            return;
        }
        int r = idx / cols, c = idx % cols;
        if (blocked[r][c]) { bt(idx + 1); return; }

        if (c + 1 < cols && !blocked[r][c + 1]) {
            blocked[r][c] = blocked[r][c + 1] = true;
            curr.push_back({ r0 + r, c0 + c, r0 + r, c0 + c + 1 });
            bt(idx + 1);
            curr.pop_back();
            blocked[r][c] = blocked[r][c + 1] = false;
        }

        if (r + 1 < rows && !blocked[r + 1][c]) {
            blocked[r][c] = blocked[r + 1][c] = true;
            curr.push_back({ r0 + r, c0 + c, r0 + r + 1, c0 + c });
            bt(idx + 1);
            curr.pop_back();
            blocked[r][c] = blocked[r + 1][c] = false;
        }
        };

    bt(0);
    return res;
}

// Generate all crossing dominoes across a vertical or horizontal split
vector<vector<Domino>> generateBoundaryDominoes(int r0, int r1, int c0, int c1, bool vertical, int split) {
    vector<vector<Domino>> res;
    int len = vertical ? r1 - r0 : c1 - c0;
    vector<bool> used(len, false);
    vector<Domino> curr;

    function<void(int)> bt = [&](int i) {
        if (i == len) {
            res.push_back(curr);
            return;
        }
        if (used[i]) { bt(i + 1); return; }

        int r = r0 + (vertical ? i : split - 1);
        int c = c0 + (vertical ? split - 1 : i);
        int r2 = vertical ? r : r + 1;
        int c2 = vertical ? c + 1 : c;

        used[i] = true;
        curr.push_back({ r, c, r2, c2 });
        bt(i + 1);
        curr.pop_back();
        used[i] = false;
        bt(i + 1);
        };

    bt(0);
    return res;
}

// Recursive divide and conquer function
vector<vector<Domino>> divideAndConquer(int r0, int r1, int c0, int c1) {
    int rows = r1 - r0, cols = c1 - c0;
    if (rows * cols <= 4)
        return tileRegion(r0, r1, c0, c1, {});

    bool vertical = cols > rows;
    int split = vertical ? c0 + cols / 2 : r0 + rows / 2;

    auto boundaries = generateBoundaryDominoes(r0, r1, c0, c1, vertical, split);
    vector<vector<Domino>> all;

    for (const auto& bridge : boundaries) {
        auto left = vertical
            ? tileRegion(r0, r1, c0, split, bridge)
            : tileRegion(r0, split, c0, c1, bridge);
        auto right = vertical
            ? tileRegion(r0, r1, split, c1, bridge)
            : tileRegion(split, r1, c0, c1, bridge);

        for (const auto& l : left)
            for (const auto& r : right) {
                vector<Domino> full = bridge;
                full.insert(full.end(), l.begin(), l.end());
                full.insert(full.end(), r.begin(), r.end());
                if (isValidTiling(full, rows, cols, r0, c0))
                    all.push_back(full);
            }
    }

    return all;
}

void Solve() {
    auto solutions = divideAndConquer(0, N, 0, M);
    for (const auto& s : solutions) {
        ++totalTilings;
        PrintTiling(s);
    }
    cout << "Total valid tilings: " << totalTilings << "\n";
}

int main() {
    cout << "Enter rows (N) and columns (M): ";
    cin >> N >> M;
    if (N * M % 2 != 0) {
        cout << "Error: Cannot tile an odd-sized grid.\n";
        return 1;
    }
    Solve();
    return 0;
}