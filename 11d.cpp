#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#define INF 0x7fffffffffffffff

using namespace std;

struct Edge {
    int to;
    long long weight;
};
struct Vertex {
    std::vector<Edge> in, out;
    long long int distance = INF;
    bool banned = false;
    bool visited = false;
    int tag = 0;
};


std::vector<Vertex> vertices;
int start;

void readInput(char const* fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);

    int verticesCount, edgesCount;

    file >> verticesCount >> edgesCount >> start;
    start--;

    vertices.resize(verticesCount);

    for(int i = 0; i < edgesCount; i++){
        int from, to;
        long long int weight;
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

void ban(Vertex& v){
    v.tag = 3;
    v.banned = true;
    for(Edge& e : v.out){
        Vertex& other = vertices[e.to];
        if(other.tag != 3){
            ban(other);
        }
    }
}
void visitOrBan(Vertex& v){
    v.tag = 1;
    for(Edge& e : v.out){
        if(v.banned)
            return;
        Vertex& other = vertices[e.to];
        if(!other.banned && other.tag == 0 || (other.tag == 2 && other.distance > v.distance + e.weight)){
            other.distance = v.distance + e.weight;
            visitOrBan(other);
        } else if(other.tag == 1 && other.distance > v.distance + e.weight) {
            ban(v);
            return;
        }
    }
    v.tag = 2;
}

int main() {
    readInput("path.in");


    vertices[start].distance = 0;
    visitOrBan(vertices[start]);

    std::fstream file;
    file.open("path.out", std::ios::out);

    for(Vertex& v : vertices){
        if(v.banned)
            file << "-\n";
        else if(v.distance == INF){
            file << "*\n";
        } else
            file << v.distance << "\n";
    }

    file.close();

    return 0;
}