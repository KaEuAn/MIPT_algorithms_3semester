#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <cstdio>


using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t;
using u64 = uint64_t;
const int k = 26;
const char first = 'a';

struct Vertex {
    u32 next[k];
    u32 go[k];
    char s;
    u32 pred;
    u32 isLeaf;
    u32 link;
    u32 up;

    Vertex(const char &st = 0, u32 pr = 1) : s(st), pred(pr), isLeaf(0), link(0), up(0) {}
};
struct simpleVertex{
    vector<std::pair<u32, char>> x;
    simpleVertex(u32 n) {
        x.reserve(n);
        for (int j = 0; j < n; ++j) {
            u32 b;
            char c;
            cin >> b >> c;
            char nowChar = c - 'a';
            x.push_back( std::make_pair(b, nowChar));
        }
    }
};

vector<simpleVertex> vertexies1;
vector<Vertex> vertexies2;
u32 size2 = 2;
void addString(const string& s) {
    u32 nowVert = 1;
    for (u32 i = 0; i < s.size(); ++i) {
        char nowChar = s[i] - first;
        if (vertexies2[nowVert].next[nowChar] == 0) {
            vertexies2.push_back(Vertex(nowChar, nowVert));
            u32 newVertex = size2;
            memset(vertexies2[size2].next, 0, sizeof(vertexies2[size2].next));
            memset(vertexies2[size2].go, 0, sizeof(vertexies2[size2].go));
            vertexies2[nowVert].next[nowChar] = newVertex;
            ++size2;
        }
        nowVert = vertexies2[nowVert].next[nowChar];
    }
    ++vertexies2[nowVert].isLeaf;
}

u32 getLink(vector<Vertex>& vertexies, u32, char);

u32 getSuffLink(vector<Vertex>& vertexies, u32 curVer) {
    if (vertexies[curVer].link == 0) {
        if(curVer == 1 || vertexies[curVer].pred == 1)
            vertexies[curVer].link = 1;
        else
            vertexies[curVer].link = getLink(vertexies, getSuffLink(vertexies, vertexies[curVer].pred), vertexies[curVer].s);
    }
    return vertexies[curVer].link;
}

u32 getLink(vector<Vertex>& vertexies, u32 curVer, char nowChar) {
    if (vertexies[curVer].go[nowChar] == 0) {
        if (vertexies[curVer].next[nowChar] != 0)
            vertexies[curVer].go[nowChar] = vertexies[curVer].next[nowChar];
        else if (curVer == 1)
            vertexies[curVer].go[nowChar] = 1;
        else
            vertexies[curVer].go[nowChar] = getLink(vertexies, getSuffLink(vertexies, curVer), nowChar);
    }
    return vertexies[curVer].go[nowChar];
}

u32 getUp(vector<Vertex>& verticies, u32 curVer) {
    if (verticies[curVer].up == 0) {
        u32 su = getSuffLink(verticies, curVer);
        if (verticies[su].isLeaf)
            verticies[curVer].up = su;
        else if (su == 1)
            verticies[curVer].up = 1;
        else
            verticies[curVer].up = getUp(verticies, su);

    }
    return verticies[curVer].up;
}

u64 count = 0;
void dfs(u32 firstVer, u32 secondVer) {
    u32 sn = secondVer;
    while (sn != 1) {
        count += vertexies2[sn].isLeaf;
        sn = getUp(vertexies2, sn);
    }
    for (auto it : vertexies1[firstVer].x) {
        dfs(it.first, getLink(vertexies2, secondVer, it.second));
    }
}

int main() {
    u32 n;
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    std::cout.tie(0);
    cin >> n;
    vertexies1.reserve(n + 1);
    vertexies1.push_back(simpleVertex(0));
    for (u32 i = 1; i < n + 1; ++i) {
        u32 a;
        cin >> a;
        vertexies1.push_back(simpleVertex(a));
    }
    cin >> n;
    vertexies2.reserve(100000);
    vertexies2.push_back(Vertex());
    vertexies2.push_back(Vertex());
    memset(vertexies2[1].next, 0, sizeof(vertexies2[1].next));
    memset(vertexies2[1].go, 0, sizeof(vertexies2[1].go));
    for (int i = 0; i < n; ++i) {
        string input;
        cin >> input;
        addString(input);
    }
    dfs(1, 1);
    cout << count;
    return 0;
}