#include <iostream>
#include <vector>

using namespace std;

int* prefixFunction(string const& str) {
    auto length = str.length();
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

int** buildAutomat(int size, string const& pattern){
    auto patternLength = pattern.length();

    auto variants = new int*[patternLength+1];
    auto prefices = prefixFunction(pattern);

    variants[0] = new int[size];
    for(int i = 0; i < size; i++){
        variants[0][i] = 0;
    }
    variants[0][pattern[0]-'A'] = 1;

    for(int i = 1; i < patternLength; i++){
        variants[i] = new int[size];
        for(int j = 0; j < size; j++)
            variants[i][j] = variants[prefices[i-1]][j];
        variants[i][pattern[i] - 'A'] = i + 1;
    }

    variants[patternLength] = new int[size];
    for(int j = 0; j < size; j++)
        variants[patternLength][j] = variants[prefices[patternLength-1]][j];
    return variants;
}

vector<int> getInlines(string const& pattern, string const& str){
    auto stringLength = str.length();
    auto patternLength = pattern.length();
    auto states = buildAutomat(59, pattern);

    int state = 0;
    vector<int> inlines;
    for(int i = 0; i < stringLength; i++){
        state = states[state][str[i]-'A'];
        if(state == patternLength)
            inlines.push_back(i - patternLength + 2);
    }
    return inlines;
}

int main(){
    freopen("search1.in", "rt", stdin);
    freopen("search1.out", "wt", stdout);

    string pattern, str;
    cin >> pattern >> str;

    auto inlines = getInlines(pattern, str);

    cout << inlines.size() << endl;
    for(int inline_ : inlines){
        cout << inline_ << ' ';
    }

    return 0;
}