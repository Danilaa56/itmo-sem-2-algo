#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int to;
};
struct Vertex {
    std::vector<Edge> out;
    bool visited = false;
    long long int maxOff = 0;
    long long int maxOn = 0;
    long long int max = 0;
};


int start;
std::vector<Vertex> vertices;

void readInput() {
    int verticesCount;
    cin >> verticesCount;
    for(int i = 0; i < verticesCount; i++){
        Vertex v;
        vertices.push_back(v);
    }
    for(int i = 0; i < verticesCount; i++){
        int parent;
        cin >> parent;
        if(parent != 0){
            vertices[parent-1].out.push_back(Edge{i});
        } else {
            start = i;
        }
    }
}

void visit(Vertex& vertex){
    vertex.visited = true;
    vertex.maxOff = 0;
    vertex.maxOn = 1;

    for(Edge edge : vertex.out){
        if(!vertices[edge.to].visited)
            visit(vertices[edge.to]);
        vertex.maxOff += vertices[edge.to].max;
        vertex.maxOn += vertices[edge.to].maxOff;
    }

    vertex.max = vertex.maxOff > vertex.maxOn ? vertex.maxOff : vertex.maxOn;
}

int main(){
    readInput();

    visit(vertices[start]);

    cout << vertices[start].max;

    return 0;
}