#include <iostream>
#include <vector>
#include <queue>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = -1;
};

int main() {
    FILE* file = fopen("pathbge1.in", "rt");

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

    std::queue<Vertex*> q;
    q.push(&vertices[0]);

    vertices[0].tagNumber = 0;

    while(!q.empty()){
        Vertex* v = q.front();
        q.pop();

        int tagNumber = v->tagNumber + 1;

        for(Vertex* verticesToTag : v->connected){
            if(verticesToTag->tagNumber != -1)
                continue;
            verticesToTag->tagNumber = tagNumber;
            q.push(verticesToTag);
        }
    }

    file = fopen("pathbge1.out", "wt");
    for(int i = 0; i < numVertices; i++){
        fprintf(file, "%d ", vertices[i].tagNumber);
    }
    fclose(file);

    return 0;
}