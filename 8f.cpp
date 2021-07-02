#include <iostream>
#include <vector>
#include <queue>

struct Vertex{
    std::vector<Vertex*> connected;
    int tagNumber = -1;
    int x, y;
    Vertex* left = nullptr;
    Vertex* right = nullptr;
    Vertex* top = nullptr;
    Vertex* bottom = nullptr;
};

int main() {
    FILE* file = fopen("input.txt", "rt");

    int height, width;

    fscanf(file, "%d %d", &height, &width);

    int verticesCount = height*width;

    Vertex vertices[height][width];

    int startX, startY;
    int exitX, exitY;

    for(int i = 0; i < height; i++){
        char c;
        fscanf(file, "\r\n\r");
        for(int j = 0; j < width; j++) {
            fscanf(file, "%c", &c);
            switch (c) {
                case 'T':
                    vertices[i][j].tagNumber = -2;
                    exitX = j;
                    exitY = i;
                    break;
                case '.':
                    vertices[i][j].tagNumber = -1;
                    break;
                case 'S':
                    vertices[i][j].tagNumber = 0;
                    startX = j;
                    startY = i;
                    break;
                default:
                    vertices[i][j].tagNumber = 2147000000;
                    break;
            }
        }
    }
    fclose(file);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++) {
            if(i>0 && vertices[i-1][j].tagNumber != 2147000000)
                vertices[i][j].connected.push_back(&vertices[i-1][j]);
            if(j>0 && vertices[i][j-1].tagNumber != 2147000000)
                vertices[i][j].connected.push_back(&vertices[i][j-1]);
            if(i+1<height && vertices[i+1][j].tagNumber != 2147000000)
                vertices[i][j].connected.push_back(&vertices[i+1][j]);
            if(j+1<width && vertices[i][j+1].tagNumber != 2147000000)
                vertices[i][j].connected.push_back(&vertices[i][j+1]);
        }
    }

    std::queue<Vertex*> q;
    q.push(&vertices[startY][startX]);

    bool exitFound = false;

    while(!q.empty() && !exitFound){
        Vertex* v = q.front();
        q.pop();

        int distance = v->tagNumber + 1;

        for(Vertex* verticesToTag : v->connected){
            if(verticesToTag->tagNumber == -2){
                verticesToTag->tagNumber = distance;
                exitFound = true;
                break;
            }
            if(verticesToTag->tagNumber != -1)
                continue;
            verticesToTag->tagNumber = distance;
            q.push(verticesToTag);
        }
    }

    file = fopen("output.txt", "wt");
    if(exitFound){
        int distance = vertices[exitY][exitX].tagNumber;
        fprintf(file, "%d\n", distance);

        char pathString[distance+1];
        pathString[distance] = 0;

        int x = exitX;
        int y = exitY;
        while(distance > 0){
            distance--;
            if(x>0 && vertices[y][x-1].tagNumber == distance){
                pathString[distance] = 'R';
                x--;
                continue;
            }
            if(x+1<width && vertices[y][x+1].tagNumber == distance){
                pathString[distance] = 'L';
                x++;
                continue;
            }
            if(y>0 && vertices[y-1][x].tagNumber == distance){
                pathString[distance] = 'D';
                y--;
                continue;
            }
            if(y+1<height && vertices[y+1][x].tagNumber == distance){
                pathString[distance] = 'U';
                y++;
                continue;
            }
        }
        fprintf(file, "%s", pathString);
    } else {
        fprintf(file, "%d", -1);
    }
    fclose(file);

    return 0;
}