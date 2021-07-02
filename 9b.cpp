#include <iostream>
#include <vector>
#include <stack>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = -1;
    int index = 0;
};

bool exitFound = false;
std::stack<Vertex*> outputStack;

bool goThrough(Vertex& v){
    v.tagNumber = 0;
    for(Vertex* connectedVertex : v.connected){
        if(connectedVertex->tagNumber == 0){
            connectedVertex->tagNumber = 2;
            outputStack.push(&v);
            return true;
        }
        if(connectedVertex->tagNumber == -1)
            if(goThrough(*connectedVertex)) {
                if(!exitFound)
                    outputStack.push(&v);
                if(v.tagNumber == 2){
                    exitFound = true;
                }
                return true;
            }
    }
    v.tagNumber = 1;
    return false;
}

int main() {
    FILE* file = fopen("cycle.in", "rt");

    int numVertices, numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);

    Vertex vertices[numVertices];
    for(int i = 0; i < numVertices; i++){
        vertices[i].index = i+1;
    }

    for(int i = 0; i < numEdges; i++){
        int index1, index2;
        fscanf(file, "%d %d", &index1, &index2);
        if(index1 == index2)
            continue;
        index1--;
        index2--;
        vertices[index1].connected.push_back(&vertices[index2]);
        //vertices[index2].connected.push_back(&vertices[index1]);
    }

    fclose(file);

    bool hasCycle = false;

    for(int i = 0; i < numVertices; i++){
        if(vertices[i].tagNumber == -1)
            if(goThrough(vertices[i])){
                hasCycle = true;
                break;
            }
    }

    file = fopen("cycle.out", "wt");

    if(!hasCycle){
        fprintf(file, "NO");
    } else {
        fprintf(file, "YES\n");
        for(int i = outputStack.size(); i > 0; i--){
            Vertex* v = outputStack.top();
            outputStack.pop();
            fprintf(file, "%d ", v->index);
        }
    }

    fclose(file);



    return 0;
}