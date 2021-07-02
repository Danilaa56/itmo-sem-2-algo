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

int** buildAutomat(int size, string& pattern, size_t length){

    auto variants = new int*[length+1];
    auto prefices = prefixFunction(pattern, length);

    variants[0] = new int[size];
    for(int i = 0; i < size; i++){
        variants[0][i] = 0;
    }
    variants[0][pattern[0]-'a'] = 1;

    for(int i = 1; i < length; i++){
        variants[i] = new int[size];
        for(int j = 0; j < size; j++)
            variants[i][j] = variants[prefices[i-1]][j];
        variants[i][pattern[i] - 'a'] = i + 1;
    }

    variants[length] = new int[size];
    for(int j = 0; j < size; j++)
        variants[length][j] = variants[prefices[length-1]][j];
    return variants;
}

int main(){
    //freopen("input.txt", "rt", stdin);

    int size = 0;
    string pattern;
    cin >> size >> pattern;
    auto length = pattern.length();

    auto variants = buildAutomat(size, pattern, length);

    for(int i = 0; i <= length; i++){
        for(int j = 0; j < size; j++)
            cout << variants[i][j] << " ";
        cout << endl;
    }

    return 0;
}