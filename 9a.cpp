#include <iostream>
#include <vector>
#include <stack>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = -1;
    int index = 0;
};

int numVertices = 0;
Vertex vertices[200000];
std::stack<Vertex*> outputStack;

void readVerticesAndEdges(char const* fileName){
    FILE* file = fopen(fileName, "rt");

    int numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);

    for(int i = 0; i < numVertices; i++){
        vertices[i].index = i+1;
    }

    for(int i = 0; i < numEdges; i++){
        int index1, index2;
        fscanf(file, "%d %d", &index1, &index2);
        index1--;
        index2--;
        vertices[index1].connected.push_back(&vertices[index2]);
        //vertices[index2].connected.push_back(&vertices[index1]);
    }

    fclose(file);
}

bool goThrough(Vertex* vertex){
    vertex->tagNumber = 0;

    for(Vertex* connectedVertex : vertex->connected){
        switch (connectedVertex->tagNumber){
            case -1:
                if(!goThrough(connectedVertex)){
                    return false;
                }
                break;
            case 0:
                return false;
            case 1:
                continue;
        }
    }

    vertex->tagNumber = 1;
    outputStack.push(vertex);
    return true;
}

int main() {
    readVerticesAndEdges("topsort.in");

    bool error = false;

    for(int i = 0; i < numVertices; i++){
        if(vertices[i].tagNumber == -1){
            if(!goThrough(&vertices[i])){
                error = true;
                break;
            }
        }
    }

    FILE* file = fopen("topsort.out", "wt");

    if(error){
        fprintf(file, "-1");
    } else {
        for(int i = 0; i < numVertices; i++){
            Vertex* v = outputStack.top();
            outputStack.pop();
            fprintf(file, "%d ", v->index);
        }
    }

    fclose(file);

    return 0;
}