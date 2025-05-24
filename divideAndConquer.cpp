#include <iostream>
using namespace std;

// Simple divide and conquer for 2 x n domino tiling
int divideAndConquerTiling(int n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    return divideAndConquerTiling(n - 1) + divideAndConquerTiling(n - 2);
}

int main()
{   
    int n;
    cout << "Enter the value of n (for 2 x n grid): ";
    cin >> n;
    int result = divideAndConquerTiling(n);
    cout << "Number of ways to tile 2 x " << n << " grid: " << result << endl;
    return 0;
}