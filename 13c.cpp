#include <iostream>

using namespace std;

int* prefixFunction(string const& str, size_t length) {
    int* prefixLength = new int[length];
    prefixLength[0] = 0;

    for(int i = 1; i < length; i++){
        int currentLength = prefixLength[i - 1];
        while (currentLength > 0 && str[i] != str[currentLength])
            currentLength = prefixLength[currentLength-1];
        if(str[i] == str[currentLength]){
            currentLength++;
        }
        prefixLength[i] = currentLength;
    }

    return prefixLength;
}

int main(){
    freopen("prefix.in", "rt", stdin);
    freopen("prefix.out", "wt", stdout);

    string pattern;
    cin >> pattern;
    auto length = pattern.length();

    int* prefices = prefixFunction(pattern, length);

    for(int i = 0; i < length; i++){
        cout << prefices[i] << " ";
    }

    return 0;
}