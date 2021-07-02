#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

struct Edge {
    int to;
    int weight;
};
struct Vertex {
    std::vector<Edge> in, out;
    long long int distance = 0x7fffffffffffff;
    bool used = false;
};


std::vector<Vertex> vertices;

void readInput(char const* fileName) {
    //ofstream.open

    std::fstream file;
    file.open(fileName, std::ios::in);

    int verticesCount, edgesCount;

    file >> verticesCount >> edgesCount;

    for(int fromIndex = 0; fromIndex < verticesCount; fromIndex++){
        Vertex vertex;
        vertices.push_back(vertex);
    }

    for(int i = 0; i < edgesCount; i++){
        int from, to, weight;
        file >> from >> to >> weight;
        vertices[from-1].out.push_back(Edge{to-1, weight});
        vertices[to-1].out.push_back(Edge{from-1, weight});

        vertices[from-1].in.push_back(Edge{to-1, weight});
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

void dijkstra(Vertex& s) {
    s.distance = 0;
    priority_queue<int, vector<int>, compare> q;

    auto isInQueue = new bool[vertices.size()];
    for(int i = vertices.size()-1; i >= 0; i--)
        isInQueue[i] = false;
    q.push(0);
    isInQueue[0] = true;

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
    readInput("pathbgep.in");

    dijkstra(vertices[0]);

    std::fstream file;
    file.open("pathbgep.out", std::ios::out);

    auto length = vertices.size();
    for(Vertex& v : vertices){
        file << (v.distance == 0x7fffffffffffffff ? -1 : v.distance) << " ";
    }

    file.close();

    return 0;
}