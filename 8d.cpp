#include <iostream>
#include <vector>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = 0;
    char tagged = 0;
};

void tagRecursively(Vertex& vertex, int tagNumber){
    vertex.tagNumber = tagNumber;
    vertex.tagged = 1;
    for(Vertex* v : vertex.connected){
        if(v->tagged)
            continue;
        tagRecursively(*v, tagNumber);
    }
}

int main() {
    FILE* file = fopen("components.in", "rt");

    int numVertices, numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);

    Vertex vertices[numVertices];

    for(int i=0; i<numEdges; i++){
        int index1, index2;
        fscanf(file, "%d %d", &index1, &index2);
        if(index1 == index2)
            continue;
        index1--;
        index2--;
        vertices[index1].connected.push_back(&vertices[index2]);
        vertices[index2].connected.push_back(&vertices[index1]);
    }

    fclose(file);

    int componentNum = 0;
    for(int i=0; i<numVertices; i++){
        if(vertices[i].tagged)
            continue;
        tagRecursively(vertices[i], ++componentNum);
    }

    file = fopen("components.out", "wt");
    fprintf(file, "%d\n", componentNum);
    for(int i = 0; i < numVertices; i++){
        fprintf(file, "%d ", vertices[i].tagNumber);
    }
    fclose(file);

    return 0;
}