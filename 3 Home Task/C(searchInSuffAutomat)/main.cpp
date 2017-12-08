#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cstdio>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::map;
using u32 = uint64_t;
using u8 = uint16_t;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;

struct SuffixAutomat {

    struct Vertice {
        u32 len;
        Vertice* suff;
        map<char, shared_ptr<Vertice>> children;

        Vertice() : Vertice(0) {}
        Vertice(u32 le) : Vertice(le, nullptr) {}
        Vertice(u32 le, Vertice* su) : len(le), suff(su) {}
        Vertice(Vertice* v, u32 le) :  Vertice(le, v->suff) {
            children = v->children;
        }
    };
    shared_ptr<Vertice> start;
    shared_ptr<Vertice> lastString;

    void addChar(char newSimbol) {
        auto newLast = make_shared<Vertice>(lastString->len + 1);
        Vertice* it = lastString.get();
        for(; it && it->children.count(newSimbol) == 0; it = it->suff) {
            it->children[newSimbol] = newLast;
        }
        if (!it)
            newLast->suff = start.get();
        else {
            Vertice* itChild = (it->children[newSimbol]).get();
            if (it->len + 1 == itChild->len)
                newLast->suff = itChild;
            else {
                auto clone = make_shared<Vertice>(itChild, it->len + 1);
                itChild->suff = clone.get();
                newLast->suff = clone.get();
                for(; it && it->children[newSimbol].get() == itChild; it = it->suff) {
                    it->children[newSimbol] = clone;
                }
            }
        }
        lastString = newLast;
    }


    Vertice* step(Vertice* v, char c) const {
        if (v->children.count(c)) {
            return v->children[c].get();
        }
        return nullptr;
    }

};


bool isLatin(const string& str, u32 i) {
    return (str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A');
}

char convert(char s) {
    if (s <= 'z' && s >= 'a') {
        s += 'A' - 'a';
    }
    return s;
}

int main() {
    freopen("nenokku.in", "r", stdin);
    freopen("nenokku.out", "w", stdout);
    SuffixAutomat au;
    au.start = au.lastString = make_shared<SuffixAutomat::Vertice>();
    while(! cin.eof()) {
        string str;
        getline(cin, str);
        if (str.length() < 2)
            continue;
        u32 i = 2;
        if (str[0] == '?') {
            SuffixAutomat::Vertice* x = au.start.get();
            bool flag = true;
            for (; i < str.size(); ++i) {
                char xxx = str[i];
                if (isLatin(str, i)) {
                    xxx = convert(xxx);
                }
                if(! au.step(x, xxx)) {
                    flag = false;
                    break;
                }
            }
            if (flag)
                cout << "YES\n";
            else
                cout << "NO\n";
        } else {
            for (; i < str.size(); ++i) {
                au.addChar(convert(str[i]));
            }
        }
    }
}