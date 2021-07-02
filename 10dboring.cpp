#include <iostream>
#include <vector>
#include <list>
#include <ctime>
#include <stack>
#include <map>

using namespace std;

class Vertex;

class Edge;
class Edge{
public:
    Edge(Vertex* from, Vertex* to, int weight = 0) : from(from), to(to), weight(weight){}

    Vertex* from;
    Vertex* to;
    int weight;
};
class Vertex {
public:
    std::vector<Edge*> in, out;
    int tag = 0;
};

vector<Vertex*> inputVertices;
vector<Edge*> inputEdges;

Vertex* vertexBuffer;
int vertexBufferIndex = -1;
Edge* edgesBuffer;
int edgesBufferIndex = -1;

void readInput(char const* fileName) {
    edgesBuffer = (Edge*) malloc(sizeof(Edge) * 10000000);
    vertexBuffer = (Vertex*) malloc(sizeof(Vertex) * 1000000);
    FILE* file = fopen(fileName, "rt");

    int verticesCount, edgesCount;

    fscanf(file, "%d %d", &verticesCount, &edgesCount);

    for(int i = 0; i < verticesCount; i++){
        inputVertices.push_back(&vertexBuffer[++vertexBufferIndex]);
    }

    for(int i = 0; i < edgesCount; i++){
        int start, finish, weight;
        fscanf(file, "%d %d %d", &start, &finish, &weight);
        start--;
        finish--;

        auto edge = &edgesBuffer[++edgesBufferIndex];
        edge->from = inputVertices[start];
        edge->to = inputVertices[finish];
        edge->weight = weight + 1000000001;
        inputEdges.push_back(edge);
        inputVertices[start]->out.push_back(edge);
        inputVertices[finish]->in.push_back(edge);
    }

    fclose(file);
}
void clear(){
    for(Vertex* vertex : inputVertices)
        delete vertex;
    for(Edge* edge : inputEdges)
        delete edge;
    inputVertices.clear();
    inputEdges.clear();
    free(edgesBuffer);
    free(vertexBuffer);
}

int condensationForward(Vertex* vertex, std::stack<Vertex*>& backStack){
    int visited = 1;
    vertex->tag = 1;

    for(Edge* edges : vertex->out){
        if(edges->to->tag == 0)
            visited += condensationForward(edges->to, backStack);
    }

    vertex->tag = 2;
    backStack.push(vertex);
    return visited;
}
void condensationBackward(Vertex* vertex, int condNum){
    vertex->tag = condNum;
    for(Edge* edge : vertex->in){
        if(edge->from->tag == -1)
            condensationBackward(edge->from, condNum);
    }
}
int condensation(Vertex* root, int& visited, vector<Vertex*>& vertices, vector<Edge*>& edges){
    std::stack<Vertex*> backStack;

    int i = 0;
    for(Vertex* vertex : vertices){
        i++;
        vertex->out.clear();
        vertex->in.clear();
        vertex->tag = 0;
    }

    for(Edge* edge : edges){
        edge->from->out.push_back(edge);
        edge->to->in.push_back(edge);
    }

    visited = condensationForward(root, backStack);
    for(Vertex* vertex : vertices){
        if(vertex->tag == 0)
            condensationForward(vertex, backStack);
    }

    int condNum = -1;
    for(Vertex* vertex : vertices){
        vertex->tag = condNum;
    }

    while(!backStack.empty()){
        Vertex* vertex = backStack.top();
        backStack.pop();
        if(vertex->tag == -1){
            condNum++;
            condensationBackward(vertex, condNum);
        }
    }

    return condNum + 1;
}

long long int findMST(vector<Edge*>& edges, vector<Vertex*>& vertices, Vertex* root){
    long long int res = 0;

    std::map<Vertex*, int> minEdge;
    for(Vertex* vertex : vertices){
        minEdge[vertex] = 0x7fffffff;
    }
    for(Edge* edge : edges){
        if(minEdge[edge->to] > edge->weight)
            minEdge[edge->to] = edge->weight;
    }
    for(Vertex* vertex : vertices){
        if(vertex == root)
            continue;
        res += minEdge[vertex];
    }
    vector<Edge*> zeroEdges;
    for(Edge* edge : edges){
        if(edge->weight == minEdge[edge->to]){
            edge->weight = 0;
            zeroEdges.push_back(edge);
        }
    }

    int visited = 0;
    int componentsCount = condensation(root, visited, vertices, zeroEdges);
    if(visited == vertices.size()){
        return res;
    }
    for(Edge* edge : edges){
        if(edge->weight == 0){
            edge->weight = minEdge[edge->to];
        }
    }

    vector<Vertex*> newVertices;
    newVertices.reserve(componentsCount);
    for(int i = 0; i < componentsCount; i++)
        newVertices.push_back(&vertexBuffer[++vertexBufferIndex]);

    vector<Edge*> newEdges;
    for(Edge* edge : edges){
        if(edge->to->tag != edge->from->tag){
            Edge* newEdge = &edgesBuffer[++edgesBufferIndex];
            newEdge->from = newVertices[edge->from->tag];
            newEdge->to = newVertices[edge->to->tag];
            newEdge->weight = edge->weight - minEdge[edge->to];
            newEdges.push_back(newEdge);
            newVertices[edge->from->tag]->out.push_back(newEdge);
            newVertices[edge->to->tag]->in.push_back(newEdge);
        }
    }

    res += findMST(newEdges, newVertices, newVertices[root->tag]);

    return res;
}

void visit(int& visited, Vertex* vertex){
    visited++;
    vertex->tag = 785653413;
    for(Edge* edge : vertex->out){
        if(edge->to->tag != 785653413){
            visit(visited, edge->to);
        }
    }
}
bool isCoverable(int verticesCount, Vertex* start){
    int visited = 0;
    visit(visited, start);
    return visited == verticesCount;
}
long long int solve(){
    if(!isCoverable(inputVertices.size(), inputVertices[0]))
        return -200000004534543700;
    return findMST(inputEdges, inputVertices, inputVertices[0]) - 1000000001*((long long int)inputVertices.size() - 1);
}

int main() {

    readInput("chinese.in");
    long long int answer = solve();
    FILE* file = fopen("chinese.out", "wt");
    if(answer == -200000004534543700)
        fprintf(file, "NO\n");
    else
        fprintf(file, "YES\n%lld\n", answer);
    fclose(file);

    return 0;
}