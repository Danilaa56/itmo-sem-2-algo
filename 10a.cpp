#include <iostream>
#include <vector>

using namespace std;

struct Vertex{
    std::vector<Vertex*> in, out;
};

int numVertices = 0;
Vertex vertices[100000];

void readVerticesAndEdges(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    int numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);

    for(int i = 0; i < numEdges; i++){
        int index1, index2;
        fscanf(file, "%d %d", &index1, &index2);
        index1--;
        index2--;
        vertices[index1].out.push_back(&vertices[index2]);
        vertices[index2].in.push_back(&vertices[index1]);
    }

    fclose(file);
}

int main() {
    readVerticesAndEdges("input.txt");

    FILE* file = fopen("output.txt", "wt");
    for(int i = 0; i < numVertices; i++){
        fprintf(file, "%d ", vertices[i].in.size() + vertices[i].out.size());
    }
    fclose(file);


    return 0;
}