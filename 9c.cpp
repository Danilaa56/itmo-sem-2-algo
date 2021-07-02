#include <iostream>
#include <vector>
#include <stack>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = -1;
    bool passed = false;
};

void pass(Vertex* vertex){
    if(vertex->passed)
        return;
    vertex->passed = true;
    vertex->tagNumber = vertex->tagNumber == 1 ? 1 : 2;
    int childNumber = 3 - vertex->tagNumber;
    for(Vertex* connectedVertex : vertex->connected){
        if(!connectedVertex->passed){
            connectedVertex->tagNumber = childNumber;
            pass(connectedVertex);
        }
    }
}

Vertex vertices[100000];
int edges[200000][2];

int main() {
    FILE* file = fopen("bipartite.in", "rt");

    int numVertices, numEdges;
    fscanf(file, "%d %d", &numVertices, &numEdges);

    for(int i = 0; i < numEdges; i++){
        int index1, index2;
        fscanf(file, "%d %d", &index1, &index2);
        index1--;
        index2--;
        edges[i][0] = index1;
        edges[i][1] = index2;
        vertices[index1].connected.push_back(&vertices[index2]);
        vertices[index2].connected.push_back(&vertices[index1]);
    }

    fclose(file);

    bool bi = true;

    for(int i = 0; i < numVertices; i++){
        if(!vertices[i].passed){
            pass(&vertices[i]);
        }
    }

    for(int i = 0; i < numEdges; i++){
        if((vertices[edges[i][0]].tagNumber + vertices[edges[i][1]].tagNumber) != 3){
            bi = false;
            break;
        }
    }

    file = fopen("bipartite.out", "wt");
    fprintf(file, "%s", bi ? "YES" : "NO");
    fclose(file);

    return 0;
}