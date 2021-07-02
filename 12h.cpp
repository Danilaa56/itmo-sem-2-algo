#include <iostream>

using namespace std;

int main(){

    int n;
    cin >> n;

    int a[n+1];
    for(int i = 1; i <= n; i++){
        cin >> a[i];
    }

    int d[n+1][n+1];
    for(int i = 0; i <= n; i++){
        d[i][i] = 1;
        if(i > 0){
            d[i][i-1] = 0;
        }
    }

    for(int length = 1; length < n; length++){
        for(int i = 1; i + length <= n; i++){
            int j = i + length;
            if(a[i] == a[j]){
                d[i][j] = (d[i+1][j] + d[i][j-1] + 1) % 1000000000;
            } else {
                d[i][j] = (d[i+1][j] + d[i][j-1] - d[i+1][j-1]) % 1000000000;
                if(d[i][j] < 0){
                    d[i][j] += 1000000000;
                }
            }
        }
    }

    cout << d[1][n];

    return 0;
}