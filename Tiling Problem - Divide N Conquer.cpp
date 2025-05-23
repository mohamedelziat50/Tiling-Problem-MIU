#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Domino {
    int r1, c1, r2, c2;
};

class DominoTiler {
private:
    int N, M;
    long long totalTilings = 0;
    vector<vector<bool>> grid;

    void printTiling(const vector<Domino>& placed) {
        vector<vector<int>> display(N, vector<int>(M, 0));
        
        for (int i = 0; i < placed.size(); ++i) {
            const auto& d = placed[i];
            display[d.r1][d.c1] = i + 1;
            display[d.r2][d.c2] = i + 1;
        }

        cout << "\nTiling #" << ++totalTilings << ":\n";
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < M; ++c) {
                cout << setw(3) << (display[r][c] ? to_string(display[r][c]) : ".");
            }
            cout << '\n';
        }
        cout << '\n';
    }

    bool isValidTiling(const vector<Domino>& dominoes) {
        vector<vector<bool>> covered(N, vector<bool>(M, false));

        for (const auto& d : dominoes) {
            if (d.r1 < 0 || d.r1 >= N || d.c1 < 0 || d.c1 >= M ||
                d.r2 < 0 || d.r2 >= N || d.c2 < 0 || d.c2 >= M ||
                covered[d.r1][d.c1] || covered[d.r2][d.c2]) {
                return false;
            }
            covered[d.r1][d.c1] = covered[d.r2][d.c2] = true;
        }
        
        return true;
    }

    vector<vector<Domino>> solveRegion(int startR, int endR, int startC, int endC) {
        vector<vector<Domino>> solutions;
        int rows = endR - startR;
        int cols = endC - startC;

        // Base case: 2x1 or 1x2 region
        if (rows * cols == 2) {
            if (rows == 2) {
                solutions.push_back({{startR, startC, startR + 1, startC}});
            } else {
                solutions.push_back({{startR, startC, startR, startC + 1}});
            }
            return solutions;
        }

        // Divide the region
        bool splitVertically = (cols > rows);
        int splitR = startR + rows/2;
        int splitC = startC + cols/2;

        if (splitVertically) {
            auto leftSolutions = solveRegion(startR, endR, startC, splitC);
            auto rightSolutions = solveRegion(startR, endR, splitC, endC);

            // Combine solutions
            for (const auto& left : leftSolutions) {
                for (const auto& right : rightSolutions) {
                    vector<Domino> combined = left;
                    combined.insert(combined.end(), right.begin(), right.end());
                    if (isValidTiling(combined)) {
                        solutions.push_back(combined);
                    }
                }
            }
        } else {
            auto topSolutions = solveRegion(startR, splitR, startC, endC);
            auto bottomSolutions = solveRegion(splitR, endR, startC, endC);

            // Combine solutions
            for (const auto& top : topSolutions) {
                for (const auto& bottom : bottomSolutions) {
                    vector<Domino> combined = top;
                    combined.insert(combined.end(), bottom.begin(), bottom.end());
                    if (isValidTiling(combined)) {
                        solutions.push_back(combined);
                    }
                }
            }
        }

        return solutions;
    }

public:
    void solve(int rows, int cols) {
        N = rows;
        M = cols;
        grid.assign(N, vector<bool>(M, false));
        
        auto solutions = solveRegion(0, N, 0, M);
        
        for (const auto& solution : solutions) {
            printTiling(solution);
        }
        
        cout << "Total tilings found: " << totalTilings << '\n';
    }
};

int main() {
    int N, M;
    cout << "Enter grid dimensions (N M): ";
    cin >> N >> M;

    if ((N * M) % 2 != 0) {
        cout << "Grid must have even number of cells.\n";
        return 1;
    }

    DominoTiler tiler;
    tiler.solve(N, M);
    return 0;
}