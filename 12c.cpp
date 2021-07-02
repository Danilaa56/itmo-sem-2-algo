#include <iostream>
#define CHARACTER_BUFFER 16

using namespace std;

struct PrefixNormal{
    PrefixNormal* prev = nullptr;
    short length = 0;
    short links = 0;
    char character[CHARACTER_BUFFER];
};

int main(){

    string str1, str2;
    cin >> str1 >> str2;

    int length1 = (int) str1.length();
    int length2 = (int) str2.length();

    auto prevLine = new PrefixNormal[length2 + 1];
    auto currentLine = new PrefixNormal[length2 + 1];

    PrefixNormal* buffer = nullptr;
    for(int i = 1; i <= length1; i++){
        currentLine[0].length = 0;
        for(int j = 1; j <= length2; j++){
            if(str1[i - 1] == str2[j - 1]){
                int newLength = prevLine[j-1].length + 1;
                if(prevLine[j].length == newLength){
                    currentLine[j] = prevLine[j];
                } else if(j > 1 && prevLine[j].length + 1 == currentLine[j-1].length){
                    currentLine[j] = currentLine[j-1];
                } else {
                    PrefixNormal prevPrefix = prevLine[j-1];
                    if(prevPrefix.length == 0){
                        PrefixNormal& prefix = currentLine[j];
                        prefix.prev = nullptr;
                        prefix.character[0] = str1[i-1];
                        prefix.length = 1;
                    } else if((prevPrefix.length & (CHARACTER_BUFFER-1)) == 0){
                        buffer = new PrefixNormal();
                        *buffer = prevLine[j-1];
                        PrefixNormal& prefix = currentLine[j];
                        prefix.length = buffer->length + 1;
                        prefix.character[0] = str1[i-1];
                        prefix.prev = buffer;
                    } else {
                        PrefixNormal& prefix = currentLine[j];
                        prefix = prevLine[j-1];
                        prefix.character[prefix.length & (CHARACTER_BUFFER-1)] = str1[i-1];
                        prefix.length++;
                    }
                }
            } else if(currentLine[j-1].length >= prevLine[j].length){
                currentLine[j] = currentLine[j-1];
            } else {
                currentLine[j] = prevLine[j];
            }
        }
        if(buffer){
            buffer = nullptr;
        }
        auto tmp = currentLine;
        currentLine = prevLine;
        prevLine = tmp;
    }

    auto prefix = &prevLine[length2];
    if(prefix){
        int answer = prefix->length;
        char line[answer + 1];
        line[answer] = 0;

        while(answer){
            int length = (prefix->length - 1) & (CHARACTER_BUFFER-1);
            for(; length >= 0; length--){
                line[--answer] = prefix->character[length];
            }
            prefix = prefix->prev;
        }
        cout << line << '\n';
    } else {
        cout << '\n';
    }


    return 0;
}