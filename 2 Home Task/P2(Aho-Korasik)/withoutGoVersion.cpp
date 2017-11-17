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
const int k = 95;
const char first = 32;

struct Vertex {
    Vertex* next[k];
    char s;
    Vertex* pred;
    bool isLeaf;
    Vertex* link;
    Vertex* up;

    Vertex(const char &st = 0, Vertex* pr = nullptr) : s(st), pred(pr), isLeaf(false), link(nullptr), up(nullptr) {}
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
        }
        nowVert = nowVert->next[nowChar];
    }
    nowVert->isLeaf = true;
}

Vertex* getSuffLink(Vertex*);

Vertex* getLink(Vertex* curVert, char c) {
        if (curVert->next[c])
            return curVert->next[c];
        else if (curVert == root)
            return root;
        else
            return getLink(getSuffLink(curVert), c);
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
        if (x->isLeaf)
            curVert->up = x;
        else if(x == root)
            curVert->up = root;
        else
            curVert->up = getUp(getSuffLink(curVert));
    }
    return curVert->up;
}


int main() {
    u32 n;
    freopen("input.txt", "r", stdin);
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    std::cout.tie(0);
    cin >> n;
    root = new Vertex();
    root->link = nullptr;
    std::memset(root->next, 0, k * sizeof(Vertex*));
    string a;
    getline(cin, a);
    for (int i = 0; i < n; ++i) {
        std::getline(cin, a);
        addString(a);
    }
    std::getline(cin, a);
    do {
        Vertex* curVert = root;
        for (int i = 0; i < a.size(); ++i) {
            bool find = false;
            Vertex* newCurVert = curVert = getLink(curVert, static_cast<char>(a[i] - first));
            do {
                if (newCurVert->isLeaf) {
                    cout << a << '\n';
                    find = true;
                    break;
                }
                newCurVert = getUp(newCurVert);
            } while(newCurVert != root && newCurVert);
            if (find)
                break;
        }
        std::getline(cin, a);
    } while(!std::cin.eof());
    delete root;
    return 0;
}