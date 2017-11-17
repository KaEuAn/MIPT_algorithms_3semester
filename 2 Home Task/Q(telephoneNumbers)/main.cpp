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
using u32 = uint64_t ;
using std::unordered_map;
const int k = 10;
const int first = '0';
const u32 INF = 999999999;
unordered_map<char, uint8_t> code = {{'a', 2},{'b', 2},{'c', 2},
                                     {'i', 1},{'j', 1},{'d', 3},{'e', 3},{'f', 3},
                                     {'g', 4},{'h', 4},{'k', 5},{'l', 5},
                                     {'m', 6},{'n', 6},{'p', 7},{'r', 7},{'s', 7},
                                     {'t', 8},{'u', 8},{'v', 8},{'w', 9},{'x', 9},{'y', 9},
                                     {'o', 0},{'q', 0},{'z', 0},};



string makeDigital(const string& input){
    string answer(input.size(), '0');
    for (int i = 0; i < input.size(); ++i) {
        answer[i] = static_cast<char>(code[input[i]] + '0');
    }
    return answer;
}

struct Word
{
    string digital;
    string literal;
    Word(){}
    Word(const string& d, const string& a) : digital(d), literal(a){}
};
vector<Word*> lastWord;
vector<u32> minCount;

struct Vertex {
    Vertex* next[k];
    Vertex* go[k];
    char s;
    Vertex* pred;
    bool isLeaf;
    Vertex* link;
    Vertex* up;
    Word* word;
    u32 length;

    Vertex(const char &st = 0, Vertex* pr = nullptr, Word* w = nullptr) :
            s(st), pred(pr), isLeaf(false), link(nullptr), up(nullptr), word(w), length(0) {
        if (pr) length = pr->length + 1;
    }
    ~Vertex() {
        if (word)
            delete word;
        for (auto it: next) {
            delete it;
        }
    }
};

Vertex* root;
void addString(Word* word) {
    const string& s = word->digital;
    Vertex* nowVert = root;
    for (u32 i = 0; i < s.size(); ++i) {
        char nowChar = s[i] - first;
        if (nowVert->next[nowChar] == nullptr) {
            Vertex* newVertex = new Vertex(nowChar, nowVert);
            nowVert->next[nowChar] = newVertex;
            std::memset(newVertex->next, 0, k * sizeof(Vertex*));
            std::memset(newVertex->go, 0, k * sizeof(Vertex*));
        }
        nowVert = nowVert->next[nowChar];
    }
    nowVert->isLeaf = true;
    nowVert->word = word;
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
        if (x->isLeaf)
            curVert->up = x;
        else if(x == root)
            curVert = root;
        else
            curVert->up = getUp(getSuffLink(curVert));
    }
    return curVert->up;
}

void tryImproveAnswer(u32 i, Vertex* ver) {
    bool is;
    if (i - ver->length == -1)
        is = true;
    else
        is = minCount[i-ver->length] + 1 < minCount[i];
    if (is) {
        minCount[i] = minCount[i-ver->length] + 1;
        lastWord[i] = ver->word;
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string number;
    getline(cin, number);
    while(number[0] != '-') {
        u32 n;
        cin >> n;
        string raed;
        getline(cin, raed);
        root = new Vertex();
        root->link = root;
        std::memset(root->next, 0, k * sizeof(Vertex*));
        std::memset(root->go, 0, k * sizeof(Vertex*));
        for (int i = 0; i < n; ++i) {
            string a;
            getline(cin, a);
            Word* newWord = new Word(makeDigital(a), a);
            addString(newWord);
        }
        minCount.assign(number.length(), INF);
        lastWord.assign(number.length(), nullptr);
        Vertex* curVert = root;
        for (u32 i = 0; i < number.length(); ++i) {
            Vertex* newCurVert = curVert = getLink(curVert, static_cast<char>(number[i] - first));
            do {
                if (newCurVert && newCurVert->isLeaf) {
                    tryImproveAnswer(i, newCurVert);
                }
                newCurVert = getUp(newCurVert);
            } while(newCurVert != root && newCurVert);
        }
        if (minCount[number.length() - 1] == INF)
            cout << "No solution.\n";
        else {
            string answer = "";
            for (int i = number.length() - 1; i >= 0; i -= lastWord[i]->digital.length()) {
                answer = lastWord[i]->literal + ' ' + answer;
            }
            cout << answer << '\n';
        }
        cin >> number;
        delete root;
    }
    return 0;
}