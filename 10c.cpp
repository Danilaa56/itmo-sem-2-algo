#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct PriorityNode{
    int priority;
    int value;
};
class PriorityQueue{
public:
    explicit PriorityQueue(int bufferSize){
        array = new PriorityNode[bufferSize];
        heapified = new PriorityNode*[bufferSize];
    }

    PriorityNode* push(int value, int priority){
        array[arraySize].value = value;
        array[arraySize].priority = priority;
        arraySize++;

        heapified[size] = &array[arraySize-1];
        int i = size;
        size++;
        while(i > 0 && heapified[(i-1)/2]->priority > heapified[i]->priority){
            PriorityNode* tmp = heapified[i];
            heapified[i] = heapified[(i-1)/2];
            heapified[(i-1)/2] = tmp;
            i >>= 1;
        }

        return &array[arraySize-1];
    }

    void heapify(int index){
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int NEED = index;

        if(left < size && heapified[left]->priority < heapified[NEED]->priority){
            NEED = left;
        }
        if(right < size && heapified[right]->priority < heapified[NEED]->priority){
            NEED = right;
        }
        if(NEED != index){
            PriorityNode* tmp = heapified[NEED];
            heapified[NEED] = heapified[index];
            heapified[index] = tmp;
            heapify(NEED);
        }
    }

    void buildHeap(){
        if(!isHeap())
            for(int i = size/2; i >= 0; --i)
                heapify(i);
    }

    bool isHeap(){
        for(int i = size/2 - 1; i >= 0 ; --i){
            if(heapified[(i<<1) + 1]->priority < heapified[i]->priority){
                return false;
            }
            if((i << 1) + 2 < size && heapified[(i<<1) + 2]->priority < heapified[i]->priority){
                return false;
            }
        }
        return true;
    }

    PriorityNode* extractMin(){
        if(size == 0)
            return nullptr;

        PriorityNode* minNode = heapified[0];
        heapified[0] = heapified[--size];
        heapify(0);

        return minNode;
    }

    void decreaseKey(PriorityNode* node, int newPriority){
        if(node->priority > newPriority){
            node->priority = newPriority;
            buildHeap();
        }
    }

private:
    int size = 0;
    PriorityNode** heapified;
    PriorityNode* array;
    int arraySize = 0;
};

struct Vertex;
class Edge{
public:
    Edge(int v = 0, int weight = 0) : vertex(v), weight(weight){}

    int vertex;
    int weight;
};
struct Vertex{
    std::vector<Edge*> out;
};



int numVertices = 0;
Vertex* vertices;
Edge* edges;
int edgesWritten = 0;

void readInput(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    int numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);
    vertices = new Vertex[numVertices];
    edges = new Edge[numEdges*2];

    for(int i = 0; i < numEdges; i++){
        int start, finish, weight;
        fscanf(file, "%d %d %d", &start, &finish, &weight);
        start--;
        finish--;

        edges[edgesWritten].vertex = finish;
        edges[edgesWritten].weight = weight;
        vertices[start].out.push_back(&edges[edgesWritten++]);

        edges[edgesWritten].vertex = start;
        edges[edgesWritten].weight = weight;
        vertices[finish].out.push_back(&edges[edgesWritten++]);
    }

    fclose(file);
}

long long int solve(){
    long long int result = 0;

    PriorityNode* nodes[numVertices];

    PriorityQueue priorityQueue(numVertices);
    nodes[0] = priorityQueue.push(0, 0);

    for(int i = 1; i < numVertices; i++){
        nodes[i] = nullptr;
    }

    for(int i = 0; i < numVertices; i++){
        PriorityNode* vertexNode = priorityQueue.extractMin();
        result += vertexNode->priority;
        vertexNode->priority = -1;
        Vertex& vertex = vertices[vertexNode->value];
        int decreasedKeys = 0;
        for(Edge* edge : vertex.out){
            if(nodes[edge->vertex] == nullptr){
                nodes[edge->vertex] = priorityQueue.push(edge->vertex, edge->weight);
            } else if(nodes[edge->vertex]->priority != -1){
                if(nodes[edge->vertex]->priority > edge->weight){
                    nodes[edge->vertex]->priority = edge->weight;
                    ++decreasedKeys;
                }
            }
        }
        priorityQueue.buildHeap();
    }

    return result;
}

int main() {
    readInput("spantree3.in");

    long long int ostovWeight = solve();

    FILE* file = fopen("spantree3.out", "wt");
    fprintf(file, "%lld", ostovWeight);
    fclose(file);

    return 0;
}










