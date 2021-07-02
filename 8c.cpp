#include <iostream>

int main() {
    FILE* file = fopen("input.txt", "rt");

    int numVertices, numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);

    char matrix[numVertices][numVertices];
    for(int i=0; i<numVertices; i++){
        for(int j=0; j<numVertices; j++){
            matrix[i][j] = 0;
        }
    }
    int index1, index2;
    bool hasParallel = false;
    for(int i = 0; i < numEdges; i++){
        fscanf(file, "%d %d", &index1, &index2);
        index1--;
        index2--;
        if(matrix[index1][index2] || matrix[index2][index1]){
            hasParallel = true;
            break;
        }
        matrix[index1][index2] = 1;
        matrix[index2][index1] = 1;
    }
    fclose(file);

    file = fopen("output.txt", "wt");
    fprintf(file, "%s", hasParallel ? "YES" : "NO");
    fclose(file);

    return 0;
}
