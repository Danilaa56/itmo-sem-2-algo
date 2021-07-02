#include <iostream>
#include <vector>
#include <fstream>

class Edge {
public:
    Edge(int to, int weight) : to(to), weight(weight){}

    int to;
    int weight;
};
struct Vertex {
    std::vector<Edge*> out;
    int index = 0;
};


int start, finish;
std::vector<Vertex> vertices;

void readInput(char const* fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);

    int verticesCount;

    file >> verticesCount >> start >> finish;
    start--;
    finish--;

    for(int fromIndex = 0; fromIndex < verticesCount; fromIndex++){
        Vertex vertex;
        vertex.index = fromIndex;
        vertices.push_back(vertex);
        for(int toIndex = 0; toIndex < verticesCount; toIndex++){
            int weight;
            file >> weight;
            if(fromIndex == toIndex || weight == -1)
                continue;
            vertices[fromIndex].out.push_back(new Edge(toIndex, weight));
        }
    }
    file.close();
}

long long int dijkstra(Vertex& s, Vertex& f) {
    long long int d[vertices.size()];
    bool used[vertices.size()];

    for(Vertex& v : vertices){
        d[v.index] = 0x7fffffffffffff;
        used[v.index] = false;
    }
    d[s.index] = 0;
    for(Vertex& i : vertices){
        Vertex* v = nullptr;
        for(Vertex& j : vertices)
            if((v == nullptr || d[j.index] < d[v->index]) && !used[j.index])
                v = &j;
        if (d[v->index] == 0x7fffffffffffff)
            break;
        used[v->index] = true;
        for(Edge* e : v->out)
            if(d[v->index] + e->weight < d[e->to])
                d[e->to] = d[v->index] + e->weight;
    }
    return d[f.index];
}

long long int solve() {
    long long int result = dijkstra(vertices[start], vertices[finish]);
    return result == 0x7fffffffffffff ? -1 : result;
}

int main() {
    readInput("pathmgep.in");

    long long int answer = solve();
    FILE* file = fopen("pathmgep.out", "wt");
    fprintf(file, "%lld", answer);
    fclose(file);

    return 0;
}