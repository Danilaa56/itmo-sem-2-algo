#include <iostream>

using namespace std;

int n, m;

int solve(int** field, int y, int x){
    if(x < 0 || x >= m || y < 0 || y >= n)
        return 0;
    if(field[y][x] == -1){
        field[y][x] = solve(field, y - 2, x + 1) +
                solve(field, y - 2, x - 1) +
                solve(field, y + 1, x - 2) +
                solve(field, y - 1, x - 2);
    }
    return field[y][x];
}

int main(){
    freopen("knight2.in", "rt", stdin);
    freopen("knight2.out", "wt", stdout);

    cin >> n >> m;
    int** field = new int*[n];
    for(int j = 0; j < n; j++){
        field[j] = new int[m];
        for(int i = 0; i < m; i++)
            field[j][i] = -1;
    }
    field[0][0] = 1;
    int answer = solve(field, n - 1, m - 1);

    cout << answer;

    return 0;
}