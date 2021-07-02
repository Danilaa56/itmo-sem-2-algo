#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#define INF 0x7fffffffffffffff

using namespace std;

struct Edge {
    int to;
    int weight;
};
struct Vertex {
    std::vector<Edge> in, out;
    long long int distance = INF;
    bool used = false;
};


std::vector<Vertex> vertices;
long long int** distances;

void readInput(char const* fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);

    int verticesCount, edgesCount;

    file >> verticesCount >> edgesCount;
    distances = new long long int*[verticesCount];;

    for(int fromIndex = 0; fromIndex < verticesCount; fromIndex++){
        Vertex vertex;
        vertices.push_back(vertex);
        distances[fromIndex] = new long long int[verticesCount];
        distances[fromIndex][fromIndex] = 0;
    }

    for(int i = 0; i < edgesCount; i++){
        int from, to, weight;
        file >> from >> to >> weight;
        vertices[from-1].out.push_back(Edge{to-1, weight});
        vertices[to-1].in.push_back(Edge{from-1, weight});
    }

    file.close();
}

struct compare : public binary_function<int, int, bool>
{
    bool
    operator()(int x, int y)
    { return vertices[x].distance > vertices[y].distance; }
};

void dijkstra(int offset) {
    priority_queue<int, vector<int>, compare> q;

    auto isInQueue = new bool[vertices.size()];
    for(int i = vertices.size()-1; i >= 0; i--){
        isInQueue[i] = false;
        vertices[i].used = false;
        vertices[i].distance = INF;
    }
    vertices[offset].distance = 0;
    q.push(offset);
    isInQueue[offset] = true;

    while(!q.empty()){
        int i = q.top();
        q.pop();
        isInQueue[i] = false;
        if(vertices[i].used)
            continue;
        Vertex& v = vertices[i];
        for(Edge& e : v.out){
            if(v.distance + e.weight < vertices[e.to].distance && !vertices[e.to].used){
                vertices[e.to].distance = v.distance + e.weight;
                if(!isInQueue[e.to]){
                    isInQueue[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }
}

int main() {
    readInput("pathsg.in");

    for(int i = 0; i < vertices.size(); i++){
        dijkstra(i);
        for(int j = 0; j < vertices.size(); j++){
            distances[i][j] = vertices[j].distance;
        }
    }

    std::fstream file;
    file.open("pathsg.out", std::ios::out);

    for(int i = 0; i < vertices.size(); i++){
        for(int j = 0; j < vertices.size(); j++){
            file << distances[i][j] << " ";
        }
        file << "\n";
    }

    file.close();

    return 0;
}