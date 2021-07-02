#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
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
    int index;
};


std::vector<Vertex> vertices;

void readInput(char const* fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);

    int verticesCount;

    file >> verticesCount;

    vertices.resize(verticesCount);

    for(int from = 0; from < verticesCount; from++){
        vertices[from].index = from;
        for(int to = 0; to < verticesCount; to++){
            long long int weight;
            file >> weight;
            if(weight == 1000000000)
                continue;
            vertices[from].out.push_back(Edge{to, weight});
            vertices[to].in.push_back(Edge{from, weight});
        }
    }

    file.close();
}

bool hasCycle = false;
stack<int> cycleStack;
bool visitOrBan2(Vertex& v, int saltIn, int saltOut){
    v.tag = saltIn;
    for(Edge& e : v.out){
        Vertex& other = vertices[e.to];
        if(other.tag == 0 || (other.tag == saltOut && other.distance > v.distance + e.weight)){
            other.distance = v.distance + e.weight;
            if(visitOrBan2(other, saltIn, saltOut))
                return true;
            if(!cycleStack.empty()){
                cycleStack.push(v.index);
                if(v.tag == 4)
                    return true;
                return false;
            }

        } else if(other.tag == saltIn && other.distance > v.distance + e.weight) {
            cout << "YES\n";
            hasCycle = true;
            cycleStack.push(other.index);
            cycleStack.push(v.index);
            other.tag = 4;
            return other.index == v.index;
        }
    }
    v.tag = saltOut;
    return false;
}

int main() {
    readInput("negcycle.in");
    freopen("negcycle.out", "wt", stdout);
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].tag == 0){
            vertices[i].distance = 0;
            if(visitOrBan2(vertices[i], 2*i + 5, 2*i + 6)){
                break;
            }
        }
    }
    if(!hasCycle)
        cout << "NO\n";
    else{
        cout << cycleStack.size() << "\n";
        while(!cycleStack.empty()){
            cout << cycleStack.top() + 1 << " ";
            cycleStack.pop();
        }
    }


    return 0;
}