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
    for(int i = 0; i < numEdges; i++){
        fscanf(file, "%d %d", &index1, &index2);
        matrix[index1-1][index2-1] = 1;
    }
    fclose(file);

    file = fopen("output.txt", "wt");
    for(int i=0; i<numVertices; i++){
        for(int j=0; j<numVertices; j++){
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return 0;
}
