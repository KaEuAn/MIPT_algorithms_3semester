#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::map;
using u64 = long long;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;
using std::min;
using std::getline;

struct Vertice{
    map<char, shared_ptr<Vertice>> children;
    Vertice* parent;
    Vertice* suff;
    int length;
    u64 count;
    Vertice(Vertice* p = nullptr, Vertice* s = nullptr, int l = 0) : parent(p), suff(s), length(l), count(1) {}
};

class PalindromTree {
    shared_ptr<Vertice> oddRoot;
    shared_ptr<Vertice> evenRoot;
    shared_ptr<Vertice> maxPalSuff;
    string str;
    vector<vector<Vertice*>> sortedByLength;

    bool haveChild(Vertice* c, char symbol) const {
        if (!c)
            return false;
        return static_cast<bool>(c->children.count(symbol));
    }

    void addChar(u64 number) {
        char symbol = str[number];
        //if we go to oddEven, we will compare symbol with himself
        auto c = maxPalSuff.get();
        for (; number - 1 - c->length < 0 || symbol != str[number - 1 - c->length];
               c = c->suff) {}
        if (haveChild(c, symbol)) {
            maxPalSuff = c->children[symbol];
            ++maxPalSuff->count;
            return;
        }
        //we must add only one Vertice, maybe with only one symbol
        maxPalSuff = make_shared<Vertice>(c, c, c->length + 2);
        sortedByLength[maxPalSuff->length].push_back(maxPalSuff.get());
        c->children[symbol] = maxPalSuff;
        if (maxPalSuff->length == 1) {
            maxPalSuff->suff = evenRoot.get();
            return;
        }
        for (auto curent = maxPalSuff->parent->suff; true;
             curent = curent->suff) {
            long long x = number - 1 - curent->length;
            if (x >= 0 && str[x] == symbol) {
                maxPalSuff->suff = curent->children[symbol].get();
                return;
            }
        }
    }

public:
    PalindromTree(const string& s) : str(s) {
        sortedByLength.assign(s.length() + 4, vector<Vertice*>());
        oddRoot = make_shared<Vertice>();
        evenRoot = make_shared<Vertice>();
        oddRoot->length = -1;
        evenRoot->length = 0;
        oddRoot->count = 0;
        evenRoot->count = 0;
        evenRoot->suff = oddRoot.get();
        maxPalSuff = oddRoot;
        for (u64 i = 0; i < str.length(); ++i) {
            addChar(i);
        }
        for (int j = sortedByLength.size() - 1; j >= 0; --j) {
            for(auto it: sortedByLength[j]) {
                if (it->suff)
                    it->suff->count += it->count;
            }
        }
        evenRoot->count = 0;
        oddRoot->count = 0;
    }

    friend u64 calculateCommonPalindromes(const PalindromTree& first, const PalindromTree& second);
};

u64 calculateCommonPalindromes(shared_ptr<Vertice> one, shared_ptr<Vertice> two) {
    u64 answer = one->count * two->count;
    for(const auto& it: one->children) {
        if (two->children.count(it.first))
            answer += calculateCommonPalindromes(it.second, two->children[it.first]);
    }
    return answer;
}
u64 calculateCommonPalindromes(const PalindromTree& first, const PalindromTree& second) {
    return calculateCommonPalindromes(first.evenRoot, second.evenRoot) +
           calculateCommonPalindromes(first.oddRoot, second.oddRoot);
}


int main() {
    u64 n;
    cin >> n;
    string s;
    getline(cin, s);
    for (int i = 0; i < n; ++i) {
        string s1, s2;
        getline(cin, s1);
        getline(cin, s2);
        PalindromTree tree1(s1);
        PalindromTree tree2(s2);
        cout << "Case #" << i + 1 << ": " << calculateCommonPalindromes(tree1, tree2) << '\n';

    }
    return 0;
}