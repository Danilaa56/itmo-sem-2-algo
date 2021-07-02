#include <iostream>
#include <vector>
#include <list>
#include <ctime>

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

    void clear(){
        delete array;
        delete heapified;
    }
private:
    int size = 0;
    PriorityNode** heapified;
    PriorityNode* array;
    int arraySize = 0;
};

class Edge;

struct ListNode{
    ListNode* next;
    Edge* value;
};
class List{
public:
    int size = 0;
    ListNode* first = nullptr;
    ListNode* last = nullptr;

    bool empty() const{
        return size == 0;
    }

    void push_back(Edge* value){
        auto node = new ListNode{nullptr, value};
        if(size == 0){
            first = node;
        } else {
            last->next = node;
        }
        last = node;
        size++;
    }

    void remove(Edge* value){
        auto node = first;
        ListNode* previousNode = nullptr;
        while(node != nullptr){
            if(node->value == value){
                --size;
                if(previousNode != nullptr){
                    previousNode->next = node->next;
                } else {
                    first = node->next;
                }
                if(node == last){
                    last = previousNode;
                }
                delete node;
                break;
            }
            previousNode = node;
            node = node->next;
        }
    }

    void clear(){
        auto node = first;
        while(node != nullptr){
            auto nextNode = node->next;
            delete node;
            node = nextNode;
        }
        size = 0;
        first = nullptr;
        last = nullptr;
    }
};

struct Vertex;
class Edge{
public:
    Edge(int vertexStart = 0, int vertexFinish = 0, int weight = 0) : vertexStart(vertexStart), vertexFinish(vertexFinish), weight(weight){}

    int vertexStart;
    int vertexFinish;
    int weight;
};
struct Vertex{
    //std::vector<Edge> in, out;
    //std::list<Edge*> in, out;
    List in, out;
    //std::vector<int> lessThem;
    int tag = 0;
    //int prev = -1;
};

Edge* get(List& list, int vertexFinish){
    auto node = list.first;
    while(node != nullptr){
        Edge* edge = node->value;
        if(edge->vertexFinish == vertexFinish)
            return edge;
        node = node->next;
    }
    return nullptr;
}

int numVertices = 0;
Vertex* vertices;
Edge* edges;

void readInput(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    int numEdges;

    fscanf(file, "%d %d", &numVertices, &numEdges);
    vertices = new Vertex[numVertices];
    edges = new Edge[numEdges];

    int edgesWritten = 0;

    for(int i = 0; i < numEdges; i++){
        int start, finish, weight;
        fscanf(file, "%d %d %d", &start, &finish, &weight);
        start--;
        finish--;

        Edge* edge = get(vertices[start].out, finish);
        if(edge == nullptr){
            edges[edgesWritten].vertexStart = start;
            edges[edgesWritten].vertexFinish = finish;
            edges[edgesWritten].weight = weight;
            vertices[start].out.push_back(&edges[edgesWritten]);
            vertices[finish].in.push_back(&edges[edgesWritten]);
            edgesWritten++;
        } else {
            if(edge->weight > weight)
                edge->weight = weight;
        }
    }

    fclose(file);
}

int visited = 0;
void visit(Vertex& vertex){
    visited++;
    vertex.tag = 1;
    auto node = vertex.out.first;
    while(node != nullptr){
        if(vertices[node->value->vertexFinish].tag != 1){
            visit(vertices[node->value->vertexFinish]);
        }
        node = node->next;
    }
}
bool isCoverable(){
    visited = 0;
    visit(vertices[0]);
    return visited == numVertices;
}

long long int initialWeight;
void initialClearing(Vertex& vertex, ListNode** inNode){
    if(vertex.in.size == 1 && vertices[vertex.in.first->value->vertexStart].tag == 0){
        Edge* edgeFromParent = vertex.in.first->value;
        initialWeight += edgeFromParent->weight;
        --numVertices;

        Vertex& parent = vertices[edgeFromParent->vertexStart];

        auto node = vertex.out.first;
        while(node != nullptr){
            auto edge = node->value;

            if(edgeFromParent->vertexStart == edge->vertexFinish){
                parent.in.remove(edge);
                node = node->next;
                continue;
            }

            auto edgeFromParentToChild = get(parent.out, edge->vertexFinish);
            if(edgeFromParentToChild == nullptr){
                parent.out.push_back(edge);
            } else {
                if(edgeFromParentToChild->weight > edge->weight){
                    edgeFromParentToChild->weight = edge->weight;
                }
                vertices[edge->vertexFinish].in.remove(edge);
            }

            node = node->next;
        }
        vertex.out.clear();
        vertex.in.clear();
        if(inNode != nullptr)
            *inNode = (*inNode)->next;
        parent.out.remove(edgeFromParent);
        return;
    }

    vertex.tag = 0;

    auto node = vertex.out.first;
    while(node != nullptr){
        auto edge = node->value;
        if(vertices[edge->vertexFinish].tag != 0){
            initialClearing(vertices[edge->vertexFinish], &node);
        } else {
            node = node->next;
        }
    }

    if(inNode != nullptr){
        *inNode = (*inNode)->next;
        if(vertex.out.empty()){
            int minInWeight = 2147000000;
            node = vertex.in.first;
            while(node != nullptr){
                auto edge = node->value;

                if(edge->weight < minInWeight)
                    minInWeight = edge->weight;
                vertices[edge->vertexStart].out.remove(edge);

                node = node->next;
            }
            vertex.in.clear();
            initialWeight += minInWeight;
            numVertices--;
        }
    }
}
long long int initialClearing(){
    initialWeight = 0;
    initialClearing(vertices[0], nullptr);
    return initialWeight;
}

long long int solve(){
    if(!isCoverable())
        return -200000004534543700;

    int startVerticesCount = numVertices;
    long long int result = initialClearing();

    auto nodes = (PriorityNode**)malloc(sizeof(PriorityNode*) * startVerticesCount);

    PriorityQueue priorityQueue(numVertices);
    nodes[0] = priorityQueue.push(0, 0);

    for(int i = 1; i < startVerticesCount; i++){
        nodes[i] = nullptr;
    }

    for(int i = 0; i < numVertices; i++){
        PriorityNode* vertexNode = priorityQueue.extractMin();
        result += vertexNode->priority;
        vertexNode->priority = -1;
        Vertex& vertex = vertices[vertexNode->value];

        auto listNode = vertex.out.first;
        while(listNode != nullptr){
            auto edge = listNode->value;
            if(nodes[edge->vertexFinish] == nullptr){
                nodes[edge->vertexFinish] = priorityQueue.push(edge->vertexFinish, edge->weight);
            } else if(nodes[edge->vertexFinish]->priority != -1){
                priorityQueue.decreaseKey(nodes[edge->vertexFinish], edge->weight);
            }
            listNode = listNode->next;
        }
    }

    for(int i = 0; i < numVertices; i++){
        vertices[i].out.clear();
        vertices[i].in.clear();
    }
    priorityQueue.clear();
    delete vertices;
    delete edges;
    free(nodes);

    return result;
}
void genData(char const* fileName){
    FILE* file = fopen(fileName, "wt");

    srand(time(nullptr));

    int numVertices = rand()%1000+2;
    int numEdges = numVertices*10;

    fprintf(file, "%d %d\n", numVertices, numEdges);

    for(int i = 0; i < numEdges; i++){
        int begin = 0;
        int end = 0;
        while(begin == end || end == 1){
            begin = (rand()%numVertices) + 1;
            end = (rand()%numVertices) + 1;
        }
        fprintf(file, "%d %d %d\n", begin, end, rand()%10 + 1);
    }

    fclose(file);
}
int main() {

    if(0){
        srand(time(0));
        for(int i = 0; i < 1000; i++){
            genData("chinese.in");
            readInput("chinese.in");
            long long int answer = solve();
            if(answer == -200000004534543700)
                printf("NO\n");
            else
                printf("YES\n%lld\n", answer);
        }
    } else {
        readInput("chinese.in");
        long long int answer = solve();
        FILE* file = fopen("chinese.out", "wt");
        if(answer == -200000004534543700)
            fprintf(file, "NO\n");
        else
            fprintf(file, "YES\n%lld\n", answer);
        fclose(file);
    }

    return 0;
}