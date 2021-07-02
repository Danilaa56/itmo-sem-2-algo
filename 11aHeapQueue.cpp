#include <iostream>
#include <vector>

using namespace std;

class Vertex;
class Edge;

struct PriorityNode{
    long long int priority;
    int value;
};
class PriorityQueue {
public:
    explicit PriorityQueue(int bufferSize){
        array = new PriorityNode[bufferSize];
        heapified = new PriorityNode*[bufferSize];
    }

    PriorityNode* push(int value, long long int priority){
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
            i = (i-1)/2;
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
        for(int i = (size-1)/2; i >= 0 ; --i){
            if((i<<1) + 1 < size && heapified[(i<<1) + 1]->priority < heapified[i]->priority){
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

    void decreaseKey(PriorityNode* node, long long int newPriority){
        if(node->priority > newPriority){
            node->priority = newPriority;
            buildHeap();
        }
    }

    void clear(){
        delete array;
        delete heapified;
    }

    int size = 0;
private:
    PriorityNode** heapified;
    PriorityNode* array;
    int arraySize = 0;
};

struct ListNode{
    Edge* value;
    ListNode* next;
    ListNode* prev;
};
ListNode* listNodeBuffer = (ListNode*) malloc(sizeof(ListNode) * 2000000);
int listNodesIndex = -1;
class List{
public:
    int size = 0;
    ListNode* first = nullptr;
    ListNode* last = nullptr;

    bool empty() const{
        return size == 0;
    }

    ListNode* push_back(Edge* value){
        auto node = &listNodeBuffer[++listNodesIndex];
        node->value = value;
        node->next = nullptr;
        node->prev = last;
        if(size == 0){
            first = node;
        } else {
            last->next = node;
        }
        last = node;
        size++;
        return node;
    }

    void remove(Edge* value){
        auto node = first;
        while(node != nullptr){
            if(node->value == value){
                --size;

                if(node->prev != nullptr){
                    node->prev->next = node->next;
                } else {
                    first = node->next;
                }

                if(node == last){
                    last = node->prev;
                } else {
                    node->next->prev = node->prev;
                }

                //delete node;
                break;
            }
            node = node->next;
        }
    }

    void clear(){
        auto node = first;
        while(node != nullptr){
            auto nextNode = node->next;
            //delete node;
            node = nextNode;
        }
        size = 0;
        first = nullptr;
        last = nullptr;
    }
};



class Edge{
public:
    Edge(int from, int to, int weight = 0) : from(from), to(to), weight(weight){}

    int from, to;
    int weight;
};
class Vertex {
public:
    //std::vector<Edge*> in, out;
    List in, out;
    long long int tag = 0;
};


int start, finish;
Vertex* vertices;
int verticesCount;

void readInput(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    fscanf(file, "%d %d %d", &verticesCount, &start, &finish);
    start--;
    finish--;

    vertices = new Vertex[verticesCount];

    for(int fromIndex = 0; fromIndex < verticesCount; fromIndex++){
        for(int toIndex = 0; toIndex < verticesCount; toIndex++){
            int weight;
            fscanf(file, "%d", &weight);
            if(fromIndex == toIndex || weight == -1)
                continue;
            vertices[fromIndex].out.push_back(new Edge(fromIndex, toIndex, weight));
        }
    }

    fclose(file);
}

long long int solve(){
    PriorityQueue q(verticesCount);
    PriorityNode* nodes[verticesCount];
    for(int i = 0; i < verticesCount; i++){
        nodes[i] = nullptr;
    }
    nodes[start] = q.push(start, 0);

    long long int minWay = -1;
    bool decreasingFlag = false;
    while(q.size > 0){
        PriorityNode* minNode = q.extractMin();
        Vertex& vertex = vertices[minNode->value];
        vertex.tag = minNode->priority;
        if(minNode->value == finish) {
            minWay = vertex.tag;
            break;
        }

        auto inEdgeNode = vertex.in.first;
        while (inEdgeNode){
            Edge* edge = inEdgeNode->value;
            vertices[edge->from].out.remove(edge);
            inEdgeNode = inEdgeNode->next;
        }

        auto outEdgeNode = vertex.out.first;
        while (outEdgeNode){
            Edge* edge = outEdgeNode->value;
            int edgeTo = edge->to;
            int weight = edge->weight;
            Vertex& other = vertices[edgeTo];
            if(other.tag == -12423436096380)
                continue;
            if(nodes[edgeTo] == nullptr){
                if(decreasingFlag){
                    q.buildHeap();
                    decreasingFlag = false;
                }
                nodes[edgeTo] = q.push(edgeTo, vertex.tag + weight);
            } else if(nodes[edgeTo]->priority > vertex.tag + weight){
                nodes[edgeTo]->priority = vertex.tag + weight;
                decreasingFlag = true;
            }
            outEdgeNode = outEdgeNode->next;
        }

        if(decreasingFlag){
            q.buildHeap();
            decreasingFlag = false;
        }
        vertex.tag = -12423436096380;
    }

    return minWay;
}

int main() {
    readInput("pathmgep.in");
/*
    srand(time(0));
    for(int i = 0; i < 1000; i++){
        genData();
        long long int trueAnswer = solve0();
        for(int j = 0; j < verticesCount; j++){
            vertices[j].tag = -1;
        }
        long long int answer = solve();
        if(trueAnswer != answer){
            printf("Problem at test %d:\n", i);
            printf("Answer: %lld. Fast: %lld\n", trueAnswer, answer);
            printf("%d %d %d\n", verticesCount, start + 1, finish+1);
            for(int j = 0; j < verticesCount; j++){
                for(Edge* edge : vertices[j].out){
                    printf("%d %d %d\n", j+1, edge->to + 1, edge->weight);
                }
            }
            break;
        } else {
            printf("Passed %d\n", i);
        }
        if(trueAnswer == -1){
            printf("Bad test %d\n", i);
        }
    }*/
    long long int answer = solve();
    FILE* file = fopen("pathmgep.out", "wt");
    fprintf(file, "%d", answer);
    fclose(file);

    return 0;
}