#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <set>
#include <cmath>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::map;
using u32 = long long;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;
using std::multiset;
using std::pair;
using std::min;
using std::max;
using std::abs;

enum ActionType{
    out,
    in,
    query,
};

struct Event {
    u32 index;
    ActionType action;

    Event(u32 l, ActionType ac) : index(l), action(ac) {}

    bool operator<(const Event &other) const {
        if (index < other.index)
            return true;
        else if (index > other.index)
            return false;
        return action < other.action;
    }
};

class AnswerType {

public:

    multiset<Event> adding;
    vector<u32> answer;
    AnswerType(u32 len) {}
    AnswerType() {}
    void print() {
        for(const auto& it: answer)
            cout << it << ' ';
    }
    vector<u32> getAnswer(u32 len) {
        answer.assign(len, 0);
        for(u32 i = 0; i < len; ++i){
            adding.insert(Event(i, query));
        }
        u32 coun = 0;
        for(const auto& it: adding) {
            if (it.action == in)
                ++coun;
            else if (it.action == out)
                --coun;
            else {
                answer[it.index] = coun;
            }
        }
        return answer;
    }

};

class SolveTask {
protected:
    string sequence;
    AnswerType answer;
    bool answerWasFound;

public:
    SolveTask(): answerWasFound(false) {};
    void getData(const string& array) {
        sequence = array;
    }
    virtual void construct() = 0;
    virtual ~SolveTask() = default;
    virtual AnswerType run(const string& array) {
        getData(array);
        construct();
        return getAnswer();
    }
    virtual AnswerType getAnswer() = 0;

};

class SuffixTree : public SolveTask {
    struct Vertice {
        Vertice *suff;
        // edge to Vertice is stored as [seq[edgeStart], seq[edgeFinish])
        u32 edgeStart;
        u32 edgeFinish;
        Vertice* parent;
        u32 countLeafs;
        Vertice* maxCountLeaf;
        map<char, Vertice*> children;
        bool isLeaf;
        Vertice(u32 ss, u32 ff, Vertice* p, Vertice* s = nullptr, u32 c = 0) :
                suff(s), edgeStart(ss), edgeFinish(ff), parent(p), countLeafs(c), isLeaf(false),
        maxCountLeaf(nullptr) {}
        Vertice() : Vertice(0, 0, nullptr) {}
        Vertice*& getChild(char symbol) {
            if (!children.count(symbol))
                children[symbol] = nullptr;
            return children[symbol];
        }
        ~Vertice() {
            for(const auto& it : children) {
                delete it.second;
            }
        }

        u32 length() const {
            return edgeFinish - edgeStart - isLeaf;
        }
        u32 countLeafsFunc() const {
            if (!this)
                return 0;
            return countLeafs;
        }
    };
    struct Position {
        Vertice* vertice;
        u32 distance;
        Position(Vertice* v, u32 d) : vertice(v), distance(d) {}
        Position() : Position(nullptr, 0) {}
    };

    Vertice* start;
    Position lastNotLeaf;

    Vertice* getSuff(Vertice* v) {
        if (v->parent == nullptr)
            return start;
        if (! v->suff) {
            auto parentSuff = getSuff(v->parent);
            Position pos(parentSuff, parentSuff->length());
            u32 x = v->edgeStart + (v->parent == start);
            v->suff = split(tryGoDownInTree(pos, x, v->edgeFinish));
        }
        return v->suff;
    }

    Position tryGoDownInTree(Position pos, u32 startLetter, u32 finishLetter) {
        for (u32 i = startLetter; i < finishLetter; ) {
            //try to go throw Vertice
            if (pos.vertice->length() == pos.distance) {
                pos.vertice = pos.vertice->getChild(sequence[i]);
                pos.distance = 0;
                if (!pos.vertice)
                    return pos;
            } else {
                if (sequence[i] != sequence[pos.vertice->edgeStart + pos.distance]) {
                    pos.vertice = nullptr;
                    break;
                }
                if (finishLetter - i < pos.vertice->length() - pos.distance) {
                    pos.distance += finishLetter - i;
                    break;
                }
                i += pos.vertice->length() - pos.distance;
                pos.distance = pos.vertice->length();
            }
        }
        return pos;
    }

    Vertice* split(Position pos) {
        //if already have Vertice
        if (pos.distance == pos.vertice->length())
            return pos.vertice;
        if (pos.distance == 0)
            return pos.vertice->parent;
        auto newVerticeInTheMiddle = new Vertice(pos.vertice->edgeStart,pos.vertice->edgeStart + pos.distance,
                                                 pos.vertice->parent);
        pos.vertice->parent->getChild(sequence[pos.vertice->edgeStart]) = newVerticeInTheMiddle;
        pos.vertice->parent = newVerticeInTheMiddle;
        newVerticeInTheMiddle->getChild(sequence[pos.vertice->edgeStart + pos.distance]) = pos.vertice;
        pos.vertice->edgeStart += pos.distance;
        return newVerticeInTheMiddle;
    }

    void addChar(u32 indexSymbol) {
        for(Vertice* parentNewLeaf = nullptr; parentNewLeaf != start;) {
            Position newPos = tryGoDownInTree(lastNotLeaf, indexSymbol, indexSymbol + 1);
            if (newPos.vertice) {
                lastNotLeaf = newPos;
                break;
            }
            parentNewLeaf = split(lastNotLeaf);
            auto newLeaf = new Vertice(indexSymbol, sequence.size(), parentNewLeaf);
            lastNotLeaf.vertice = getSuff(parentNewLeaf);
            parentNewLeaf->getChild(sequence[indexSymbol]) = newLeaf;
            lastNotLeaf.distance = lastNotLeaf.vertice->length();
        }
    }


public:
    virtual void construct() override {
        start = new Vertice();
        sequence.push_back(0);
        lastNotLeaf = Position(start, 0);
        for (u32 i = 0; i < sequence.size(); ++i) {
            addChar(i);
        }
    }
    ~SuffixTree() {
        delete start;
    }
    void findCountLeafs(Vertice* v) {
        for(const auto& it: v->children) {
            findCountLeafs(it.second);
            v->countLeafs += it.second->countLeafs;
            if (v->maxCountLeaf->countLeafsFunc() < it.second->countLeafs) {
                v->maxCountLeaf = it.second;
            }
        }
        if (v->countLeafs == 0)
            v->countLeafs = 1;
    }



    multiset<u32> findAnswer(Vertice* v, u32 le) {
        u32 commonLength = le + v->length();
        if (!v->maxCountLeaf) {
            multiset<u32> ans;
            ans.insert(commonLength - 1);
            return ans;
        }
        multiset<u32> ans = findAnswer(v->maxCountLeaf, commonLength);
        for(const auto& it: v->children) {
            if (it.second != v->maxCountLeaf) {
                auto x = findAnswer(it.second, commonLength);
                for(const auto& it: x) {
                    ans.insert(it);
                }
            }
        }
        if (ans.size() < 3)
            return ans;
        u32 best = 0;
        u32 mmax = *(ans.rbegin()), mmin = *(ans.begin());
        auto yy = ans.lower_bound((mmin + mmax)/2);
        u32 update = min(abs(mmax - (*yy)), abs(mmin - (*yy)));
        if (update > best)
            best = update;
        auto next = yy;
        ++next;
        update = min(abs(mmax - (*next)), abs(mmin - (*next)));
        if (update > best)
            best = update;
        if (yy != ans.begin()) {
            auto prev = yy;
            --prev;
            update = min(abs(mmax - (*prev)), abs(mmin - (*prev)));
            if (update > best)
                best = update;
        }

        u32 final = min(max(best, le), commonLength);
        if (final > le) {
            answer.adding.insert(Event(le, in));
            answer.adding.insert(Event(final, out));
        }
        return ans;
    }

    virtual AnswerType getAnswer() override {
        findCountLeafs(start);
        findAnswer(start, 0);
        answer.getAnswer((sequence.length() - 1)/3);
        return answer;
    }
};

int main() {
    string s;
    getline(cin, s);
    SuffixTree tree;
    tree.run(s).print();
    return 0;
}