#include <iostream>

int main() {
    FILE* file = fopen("input.txt", "rt");

    int numVertices;

    fscanf(file, "%d", &numVertices);

    int matrix[numVertices][numVertices];
    for(int i=0; i<numVertices; i++){
        for(int j=0; j<numVertices; j++){
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);

    bool normal = true;
    file = fopen("output.txt", "wt");
    for(int i=0; i<numVertices; i++){
        for(int j=i; j<numVertices; j++){
            if(matrix[i][j] != matrix[j][i] || (i==j && matrix[i][j])){
                normal = false;
                break;
            }
        }
    }
    //printf("%s", normal ? "YES" : "NO");
    fprintf(file, "%s", normal ? "YES" : "NO");
    fclose(file);

    return 0;
}
