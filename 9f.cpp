#include <iostream>
#include <vector>

using namespace std;

struct Vertex{
    std::vector<Vertex*> in, out;
    int visited = 0;
};

int numVertices = 0;
Vertex vertices[100000];
int startPos;

void readVerticesAndEdges(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    int numEdges;

    fscanf(file, "%d %d %d", &numVertices, &numEdges, &startPos);

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

void whoWinsAt(Vertex* vertex){

    vertex->visited = 2; // 1 = wins here, 2 = lose here

    for(Vertex* child : vertex->out){
        if(child->visited == 0){
            whoWinsAt(child);
        }
        if(child->visited == 2){
            vertex->visited = 1;
            break;
        }
    }
}

bool solve(){
    for(int i = 0; i < numVertices; i++){
        vertices[i].visited = 0;
    }

    whoWinsAt(&vertices[startPos-1]);

    return vertices[startPos-1].visited == 1;
}

int main() {
    readVerticesAndEdges("game.in");

    FILE* file = fopen("game.out", "wt");
    fprintf(file, solve() ? "First player wins" : "Second player wins");
    fclose(file);


    return 0;
}