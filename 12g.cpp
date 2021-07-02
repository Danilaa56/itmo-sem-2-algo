#include <iostream>

using namespace std;

int main(){
    freopen("knapsack.in", "rt", stdin);
    freopen("knapsack.out", "wt", stdout);

    

    int s, n;
    cin >> s >> n;

    int w[n+1];
    for(int i = 1; i <= n; i++){
        cin >> w[i];
    }

    int d[n+1][s+1];
    for(int i = 0; i <= n; i++)
        d[i][0] = 0;
    for(int i = 0; i <= s; i++)
        d[0][i] = 0;

    for(int i = 1; i <= n; i++) {
        for(int j = 0; j <= s; j++){
            if(w[i] <= j) {
                d[i][j] = max(d[i-1][j-w[i]] + w[i], d[i-1][j]);
            } else {
                d[i][j] = d[i-1][j];
            }
        }
    }

    cout << d[n][s];


    return 0;
}