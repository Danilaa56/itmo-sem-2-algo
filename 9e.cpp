#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

struct Vertex{
    std::vector<Vertex*> in, out;
    int visited = 0;
    int visitedCount = 0;
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

int getIndex(Vertex* v){
    return (int)(((long long)v - (long long)vertices)/sizeof(Vertex));
}

bool hasCycleRecursion(Vertex* v, char* visitingFlags){
    visitingFlags[getIndex(v)] = 1;

    for(Vertex* connectedVertex : v->out){
        switch (visitingFlags[getIndex(connectedVertex)]) {
            case 1:
                return true;
            case 0:
                if(hasCycleRecursion(connectedVertex, visitingFlags)) {
                    return true;
                }
                break;
        }
    }

    visitingFlags[getIndex(v)] = 2;
    return false;
}

bool hasCycle(){
    bool hasCycle = false;

    char visitingFlags[numVertices];
    for(int i = 0; i < numVertices; i++){
        visitingFlags[i] = 0;
    }

    for(int i = 0; i < numVertices; i++){
        if(visitingFlags[i] == 0)
            if(hasCycleRecursion(&vertices[i], visitingFlags)){
                hasCycle = true;
                break;
            }
    }

    return hasCycle;
}

void loopGenVertices(int verticesCount, int edgesCount){
    numVertices = verticesCount;
    for(int i = 0; i < numVertices; i++){
        vertices[i].visited = 0;
        vertices[i].visitedCount = 0;
        vertices[i].in.clear();
        vertices[i].out.clear();
    }

    for(int i = 0; i < edgesCount; i++){
        int index1 = (rand()+rand() +rand()) % numVertices;
        int index2 = (rand()+rand() +rand() ) % numVertices;
        if(index1 == index2){
            //i--;
            continue;
        }

        char shit = false;

        //printf("%d %d\n", index1, index2);
        for(Vertex* other : vertices[index1].out){
            if(other == &vertices[index2]){
                shit = true;
                break;
            }
        }
        for(Vertex* other : vertices[index1].in){
            if(other == &vertices[index2]){
                shit = true;
                break;
            }
        }

        if(shit)
            continue;
        vertices[index1].out.push_back(&vertices[index2]);
        vertices[index2].in.push_back(&vertices[index1]);
    }
}

void genVertices(int verticesCount, int edgesCount){
    do{
        loopGenVertices(verticesCount, edgesCount);
    } while (hasCycle());
}

Vertex** list;
int currentCount = 0;

std::vector<Vertex*>* lastOut = nullptr;
Vertex* topNode = nullptr;
void pushNode(Vertex* vertex){
    list[currentCount++] = vertex;
    topNode = vertex;

    lastOut = &vertex->out;
    vertex->visitedCount = 0;
}
void popNode(){
    Vertex* vertex = list[--currentCount];
    vertex->visitedCount = false;

    if(currentCount == 0)
        return;
    topNode = list[currentCount-1];
    lastOut = &topNode->out;
}
bool solve(){
    Vertex* first = nullptr;
    Vertex* last = nullptr;

    for(int i = 0; i < numVertices; i++){
        if(vertices[i].in.empty()){
            if(first == nullptr)
                first = &vertices[i];
            else
                return false;
        }

        if(vertices[i].out.empty()){
            if(last == nullptr)
                last = &vertices[i];
            else
                return false;
        }
    }

    if(first == nullptr || last == nullptr)
        return false;

    list = new Vertex*[numVertices];

    currentCount = 0;
    pushNode(first);

    int passId = 1;

    while(currentCount > 0 && currentCount < numVertices){
        //currentIndex = getIndex(topNode)+1;
        if(topNode->visitedCount < lastOut->size()){
            Vertex* child = lastOut->operator[](topNode->visitedCount++);
            if(child->visited != passId)
                pushNode(child);
        } else {
            popNode();
            passId++;
            topNode->visited = passId;
        }
    }

    delete list;

    return currentCount == numVertices;
}

int tryDeeply(Vertex* v){
    if(v->out.empty())
        return 1;

    v->visited = 1;

    int max =  0;

    for(Vertex* connectedVertex : v->out){
        if(connectedVertex->visited != 1){
            int num = tryDeeply(connectedVertex);
            if(num > max){
                max = num;
            }
        }
    }

    v->visited = 0;
    return max + 1;
}
bool solve2(){
    Vertex* first = nullptr;
    Vertex* last = nullptr;

    for(int i = 0; i < numVertices; i++){
        if(vertices[i].in.empty()){
            if(first == nullptr)
                first = &vertices[i];
            else
                return false;
        }

        if(vertices[i].out.empty()){
            if(last == nullptr)
                last = &vertices[i];
            else
                return false;
        }
    }

    if(first == nullptr || last == nullptr)
        return false;

    int count = tryDeeply(first);

    return count == numVertices;
}

int topologicalSortLength(Vertex* v){
    if(v->out.empty()){
        v->visited = 1;
        return 1;
    }

    int max =  0;

    for(Vertex* connectedVertex : v->out){
        if(connectedVertex->visited == 0){
            topologicalSortLength(connectedVertex);
        }
        if(connectedVertex->visited > max){
            max = connectedVertex->visited;
        }
    }

    v->visited = max+1;
    return v->visited;
}
bool solve3(){
    Vertex* first = nullptr;
    Vertex* last = nullptr;

    for(int i = 0; i < numVertices; i++){
        vertices[i].visited = 0;
        if(vertices[i].in.empty()){
            if(first == nullptr)
                first = &vertices[i];
            else
                return false;
        }

        if(vertices[i].out.empty()){
            if(last == nullptr)
                last = &vertices[i];
            else
                return false;
        }
    }

    if(first == nullptr || last == nullptr)
        return false;

    int length = topologicalSortLength(first);

    return length == numVertices;
}

int main() {
    readVerticesAndEdges("hamiltonian.in");

    FILE* file = fopen("hamiltonian.out", "wt");
    fprintf(file, solve3() ? "YES" : "NO");
    fclose(file);


    return 0;
}