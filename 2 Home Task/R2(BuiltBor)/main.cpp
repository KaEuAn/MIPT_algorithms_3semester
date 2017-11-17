#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t;
using u64 = uint64_t;
const u32 INF = 9999999;


struct vertex {
    std::map<char, vertex*> next;
    string s;
    vertex* pred;
    u32 count;
    bool isLeaf;

    vertex(const string st = "", vertex* pr = nullptr) : s(st), pred(pr), count(1), isLeaf(true) {
    }
    ~vertex() {
        for(auto it : next) {
            delete it.second;
        }
    }
    string find(int number) const {
        std::map<char, vertex*>::const_iterator it = next.begin();
        if (number == 0) {
            if (! isLeaf)
                return s + it->second->find(0);
            else
                return s;
        }
        if (isLeaf)
            --number;
        while(it != next.end() && number >= 0) {
            number -= it->second->count;
            ++it;
        } --it;
        return s + it->second->find(number + it->second->count);
    }

    void add(const string& sin) {
        u32 maxInter = 0;
        for (; maxInter < std::min(sin.length(), s.length()); ++maxInter) {
            if (sin[maxInter] != s[maxInter])
                break;
        }
        if (maxInter == s.length()) {
            auto it = next.find(sin[maxInter]);
            if (it == next.end()) {
                vertex *newVertex = new vertex(string(&sin[maxInter], sin.length() - maxInter), this);
                next.insert(std::make_pair(sin[maxInter], newVertex));
            } else {
                it->second->add(string((&sin[0]) + maxInter, sin.length() - maxInter));
            }
            ++count;
        } else {
            vertex* newVertex = new vertex(string(&s[0], maxInter), pred);
            pred->next[s[0]] = newVertex;
            pred = newVertex;
            newVertex->count += count;
            newVertex->next.insert(std::make_pair(s[maxInter], this));
            s = string(&s[0] + maxInter, s.length() - maxInter);
            if (maxInter != sin.length()) {
                newVertex->isLeaf = false;
                vertex* newNewVertex = new vertex(string(&sin[0] + maxInter, sin.length() - maxInter), newVertex);
                newVertex->next.insert(std::make_pair(newNewVertex->s[0], newNewVertex));
            }
        }
    }
};

int main() {
    u32 n;
    cin >> n;
    vertex* root = new vertex;
    --root->count;
    root->isLeaf = false;
    for (int i = 0; i < n; ++i) {
        string str;
        cin >> str;
        bool isNumber = true;
        for (u32 j = 0; j < str.length(); ++j) {
            if (str[j] < '0' || str[j] > '9') {
                isNumber = false;
                break;
            }
        }
        if (isNumber) {
            cout << root->find(std::atoi(str.c_str()) - 1) << '\n';
        } else {
            root->add(str);
        }
    }
    delete root;
    return 0;
}