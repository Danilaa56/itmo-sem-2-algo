#include <iostream>
#include <list>
#include <vector>

using namespace std;

class PatternTreeNode;

class MemoryPool {

    int step = 1000;
    int last = step;
    list<PatternTreeNode*> memoryChunks;
    PatternTreeNode* lastChunk = nullptr;

public:
    PatternTreeNode* newPatternTreeNode();
};

MemoryPool pool;

class PatternTreeNode {

public:
    PatternTreeNode* children[64]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    PatternTreeNode* suffix = nullptr;
    PatternTreeNode* parent = nullptr;
    char code = 0;
    bool visited = false;
};

class PatternTree {

private:
    PatternTreeNode* root = pool.newPatternTreeNode();
    list<PatternTreeNode*> patternEnds;
public:
    void addPattern(string const& pattern) {
        auto pointer = pattern.c_str();
        auto node = root;
        while (*pointer) {
            auto& nextNode = node->children[*pointer - 'A'];
            if(!nextNode) {
                nextNode = pool.newPatternTreeNode();
                nextNode->code = *pointer - 'A';
                nextNode->parent = node;
            }
            node = nextNode;
            pointer++;
        }
        patternEnds.push_back(node);
    }

    void buildSuffices() {
        root->suffix = root;
        for(auto node : patternEnds)
            pi(node);
    }

    list<bool> check(string const& str) {
        auto node = root;
        auto pointer = str.c_str();
        while(*pointer) {
            char code = *pointer - 'A';
            node = delta(node, code);
            auto nodeToMark = node;
            while(!nodeToMark->visited) {
                nodeToMark->visited = true;
                nodeToMark = nodeToMark->suffix;
            }
            ++pointer;
        }

        list<bool> visited;
        for(auto const& patternEnd : patternEnds)
            visited.push_back(patternEnd->visited);

        return visited;
    }

private:
    PatternTreeNode* pi(PatternTreeNode* u) {
        if(!u->suffix) {
            auto v = delta(pi(u->parent), u->code);
            if(v != u)
                u->suffix = v;
            else
                u->suffix = root;
        }
        return u->suffix;
    }

    PatternTreeNode* delta(PatternTreeNode* u, char c) {
        if(u->children[c])
            return u->children[c];
        else if(u == root)
            return root;
        else
            return delta(pi(u), c);
    }
};

PatternTreeNode* MemoryPool::newPatternTreeNode() {
    if(last == step) {
        lastChunk = new PatternTreeNode[step];
        memoryChunks.push_back(lastChunk);
        last = 0;
    }
    return &lastChunk[last++];
}

void solve(list<string> const& patterns, string const& str) {
    PatternTree tree;
    for(auto const& pattern : patterns)
        tree.addPattern(pattern);
    tree.buildSuffices();

    list<bool> visitedList = tree.check(str);
    for(bool exists : visitedList)
        cout << (exists ? "YES\n" : "NO\n");
}

int main() {
    freopen("search4.in", "rt", stdin);
    freopen("search4.out", "wt", stdout);

    int count;
    cin >> count;

    list<string> patternList;
    for(int i = 0; i < count; i++) {
        string pattern;
        cin >> pattern;
        patternList.push_back(pattern);
    }

    string str;
    cin >> str;

    solve(patternList, str);

    return 0;
}