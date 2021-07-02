#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Vertex{
    int x, y;
    std::vector<Vertex*> in, out;
    int visited = 0;
};

int numVertices = 0;
Vertex* vertices;

void readInput(char const* fileName) {
    FILE* file = fopen(fileName, "rt");

    fscanf(file, "%d", &numVertices);
    vertices = new Vertex[numVertices];

    for(int i = 0; i < numVertices; i++){
        int x, y;
        fscanf(file, "%d %d", &x, &y);
        vertices[i].x = x;
        vertices[i].y = y;
    }

    fclose(file);
}

int squaredDistance(Vertex* v1, Vertex* v2){
    int xx = v1->x - v2->x;
    int yy = v1->y - v2->y;
    return xx * xx + yy * yy;
}
double solve(){
    double result = 0;

    int distances[numVertices];

    for(int i = 1; i < numVertices; i++){
        distances[i] = 2147000000;
    }

    distances[0] = -1;

    Vertex* currentVertex = &vertices[0];
    int leastIndex;

    for(int ready = 1; ready < numVertices; ready++){
        int leastDistance = 2147000000;
        for(int i = 1; i < numVertices; i++){
            if(distances[i] != -1){
                int distance = squaredDistance(currentVertex, &vertices[i]);
                if(distance < distances[i]) {
                    distances[i] = distance;
                }
                if(distances[i] < leastDistance){
                    leastIndex = i;
                    leastDistance = distances[i];
                }
            }
        }
        result += sqrt(leastDistance);
        distances[leastIndex] = -1;
        currentVertex = &vertices[leastIndex];
    }

    return result;
}

int main() {
    readInput("spantree.in");

    double ostovWeight = solve();

    FILE* file = fopen("spantree.out", "wt");
    fprintf(file, "%lf", ostovWeight);
    fclose(file);

    return 0;
}