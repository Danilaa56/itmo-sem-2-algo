#include <iostream>
#include <vector>
#include <stack>

struct Vertex{
    std::vector<Vertex*> in, out;
    int tagNumber = -1;
    int index = 0;
    int condNum = -1;
};

int numVertices = 0;
int verticesLeft;
Vertex vertices[20000];
std::stack<Vertex*> backStack;

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
        vertices[index1].out.push_back(&vertices[index2]);
        vertices[index2].in.push_back(&vertices[index1]);
    }

    fclose(file);
}

void goThrough(Vertex* vertex){
    vertex->tagNumber = 0;

    for(Vertex* connectedVertex : vertex->out){
        if(connectedVertex->tagNumber == -1){
            goThrough(connectedVertex);
        }
    }

    vertex->tagNumber = 1;
    backStack.push(vertex);
}

void goBack(Vertex* vertex, int condNum){
    vertex->condNum = condNum;
    for(Vertex* connectedVertex : vertex->in){
        if(connectedVertex->condNum == -1){
            goBack(connectedVertex, condNum);
        }
    }
}

int main() {
    readVerticesAndEdges("cond.in");
    verticesLeft = numVertices;

    for(int i = 0; i < numVertices; i++){
        if(vertices[i].tagNumber == -1){
            goThrough(&vertices[i]);
        }
    }

    int condNum = 0;

    while(!backStack.empty()){
        Vertex* vertex = backStack.top();
        backStack.pop();
        if(vertex->condNum == -1){
            condNum++;
            goBack(vertex, condNum);
        }
    }

    FILE* file = fopen("cond.out", "wt");
    fprintf(file, "%d\n", condNum);
    for(int i = 0; i < numVertices; i++)
        fprintf(file, "%d ", vertices[i].condNum);
    fclose(file);

    return 0;
}