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
const int k = 2;
const char first = '0';
enum What{
    notVisited,
    isProcessing,
    processed
};

struct Vertex {
    Vertex* next[k];
    Vertex* go[k];
    char s;
    Vertex* pred;
    bool isLeaf;
    Vertex* link;
    Vertex* up;
    What status;
    bool isBad;

    Vertex(const char &st = 0, Vertex* pr = nullptr) :
            s(st), pred(pr), isLeaf(false), link(nullptr), up(nullptr), status(notVisited), isBad(false) {}
    ~Vertex() {
        for (auto it: next) {
            delete it;
        }
    }
};

Vertex* root;
void addString(const string& s) {
    Vertex* nowVert = root;
    for (u32 i = 0; i < s.size(); ++i) {
        char nowChar = s[i] - first;
        if (! nowVert->next[nowChar]) {
            Vertex* newVertex = new Vertex(nowChar, nowVert);
            nowVert->next[nowChar] = newVertex;
            std::memset(newVertex->next, 0, k * sizeof(Vertex*));
            std::memset(newVertex->go, 0, k * sizeof(Vertex*));
        }
        nowVert = nowVert->next[nowChar];
    }
    nowVert->isLeaf = true;
    nowVert->status = processed;
    nowVert->isBad = true;
}

Vertex* getSuffLink(Vertex*);

Vertex* getLink(Vertex* curVert, char c) {
    if (! curVert->go[c]) {
        if (curVert->next[c])
            curVert->go[c] = curVert->next[c];
        else if (curVert == root)
            curVert->go[c] = root;
        else
            curVert->go[c] = getLink(getSuffLink(curVert), c);
    }
    return curVert->go[c];
}

Vertex* getSuffLink(Vertex* curVert) {
    if(! curVert->link) {
        if(curVert == root || curVert->pred == root)
            curVert->link = root;
        else
            curVert->link = getLink(getSuffLink(curVert->pred), curVert->s);
    }
    return curVert->link;
}

Vertex* getUp(Vertex* curVert) {
    if (! curVert->up) {
        Vertex* x = getSuffLink(curVert);
        if (x->isLeaf) {
            curVert->up = x;
        }
        else if(x == root)
            curVert->up = root;
        else
            curVert->up = getUp(getSuffLink(curVert));
    }
    return curVert->up;
}

bool isBad(Vertex* a) {
    if (a->status == processed || a->status == isProcessing)
        return a->isBad;
    a->status = isProcessing;
    if(isBad(getUp(a))) {
        a->isBad = true;
    }
    else if (isBad(getLink(a, 0)) && isBad(getLink(a, 1))) {
        a->isBad = true;
    }
    a->status = processed;
    return a->isBad;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    std::cout.tie(0);
    u32 n;
    cin >> n;
    root = new Vertex();
    root->link = root;
    std::memset(root->next, 0, k * sizeof(Vertex*));
    std::memset(root->go, 0, k * sizeof(Vertex*));
    for (int i = 0; i < n; ++i) {
        string a;
        cin >> a;
        addString(a);
    }
    if(isBad(root))
        cout << "NIE";
    else
        cout << "TAK";
    delete root;
    return 0;
}