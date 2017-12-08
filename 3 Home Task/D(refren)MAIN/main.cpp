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
using u32 = uint64_t;
using u8 = uint16_t;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;

class AnswerType {
public:
    u32 answer;
    u32 length;
    vector<u8> string;
public:
    AnswerType(): answer(0), length(0) {}
    void print() const {
        cout << answer << '\n' << length << '\n';
        for (const auto& it: string) {
            cout << it << ' ';
        }
    }
    friend class SuffixAutomat;
};

class SolveTask {
protected:
    vector<u8> sequence;
    AnswerType answer;
    bool answerWasFound;

public:
    SolveTask(): answerWasFound(false) {};
    void getData(const vector<u8> &array) {
        sequence = array;
    }
    virtual void construct() = 0;
    virtual ~SolveTask() = default;
    virtual void run(const vector<u8> &array) {
        getData(array);
        construct();
        getAnswer().print();
    }
    virtual AnswerType getAnswer() = 0;

};


class SuffixAutomat : public SolveTask {

    struct Vertice {
        u32 len;
        Vertice* suff;
        u32 count;
        map<u8, shared_ptr<Vertice>> children;
        bool used;

        Vertice() : Vertice(0) {}
        Vertice(u32 le) : Vertice(le, nullptr) {}
        Vertice(u32 le, Vertice* su) : len(le), suff(su), count(1), used(false) {}
        Vertice(Vertice* v, u32 le) :  Vertice(le, v->suff) {
            children = v->children;
            count = 0;
        }
    };
    shared_ptr<Vertice> start;
    shared_ptr<Vertice> lastString;
    vector<vector<Vertice*>> sortedByLength;

    void addChar(u8 newSimbol) {
        auto newLast = make_shared<Vertice>(lastString->len + 1);
        sortedByLength[newLast->len].push_back(newLast.get());
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
                sortedByLength[clone->len].push_back(clone.get());
                itChild->suff = clone.get();
                newLast->suff = clone.get();
                for(; it && it->children[newSimbol].get() == itChild; it = it->suff) {
                    it->children[newSimbol] = clone;
                }
            }
        }
        lastString = newLast;
    }


public:
    SuffixAutomat() {}
    virtual void construct() override {
        sortedByLength.assign(sequence.size() + 1, vector<Vertice*>());
        start = lastString = make_shared<Vertice>();
        start->count = 0;
        for (int i = 0; i < sequence.size(); ++i) {
            addChar(sequence[i]);
        }
        for (int j = sequence.size(); j >= 0; --j) {
            for(auto it: sortedByLength[j]) {
                it->suff->count += it->count;
            }
        }
    }
    virtual ~SuffixAutomat() override = default;

    void findLenAns(const shared_ptr<Vertice>& v) {
        v->used = true;
        if ((v->len * v->count) > answer.answer) {
            answer.answer = v->len * v->count;
            answer.length = v->len;
        }
        for(const auto& it: v->children) {
            if (! it.second->used)
                findLenAns(it.second);
        }
    }

    void findString(const shared_ptr<Vertice>& v, u32 le) {
        v->used = false;
        if ((v->len * v->count) == answer.answer) {
            answerWasFound = true;
            return;
        }
        for(const auto& it: v->children) {
            if (it.second->used && it.second->len == le + 1) {
                answer.string.push_back(it.first);
                findString(it.second, le + 1);
                if(answerWasFound)
                    return;
                answer.string.pop_back();
            }
        }
    }


    virtual AnswerType getAnswer() override {
        answer.string.reserve(sequence.size());
        findLenAns(start);
        findString(start, 0);
        return answer;
    }

};


class SuffixTree : public SolveTask {
    struct Vertice {
        Vertice *suff;
        // edge to Vertice is stored as [seq[edgeStart], seq[edgeFinish])
        u32 edgeStart;
        u32 edgeFinish;
        Vertice* parent;
        u32 count;
        map<u8, Vertice*> children;
        Vertice(u32 ss, u32 ff, Vertice* p, Vertice* s = nullptr, u32 c = 0) :
                suff(s), edgeStart(ss), edgeFinish(ff), parent(p), count(c) {}
        Vertice() : Vertice(0, 0, nullptr) {}
        Vertice*& getChild(u8 simbol) {
            if (!children.count(simbol))
                children[simbol] = nullptr;
            return children[simbol];
        }
        ~Vertice() {
            for(const auto& it : children) {
                delete it.second;
            }
        }

        u32 length() const {
            return edgeFinish - edgeStart;
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
        if (v == start)
            return v;
        if (! v->suff) {
            auto parentSuff = getSuff(v->parent);
            Position pos(parentSuff, parentSuff->length());
            u32 x = v->edgeStart;
            if (v->parent == start)
                ++x;
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
        //if already have vertice
        if (pos.distance == pos.vertice->length())
            return pos.vertice;
        if (pos.distance == 0)
            return pos.vertice->parent;
        auto newVerticeInTheMiddle = new Vertice(pos.vertice->edgeStart,pos.vertice->edgeStart + pos.distance,
                                               pos.vertice->parent);
        pos.vertice->parent = newVerticeInTheMiddle;
        newVerticeInTheMiddle->getChild(sequence[pos.vertice->edgeStart + pos.distance]) = pos.vertice;
        pos.vertice->parent->getChild(sequence[pos.vertice->edgeStart]) = newVerticeInTheMiddle;
        pos.vertice->edgeStart += pos.distance;
        return newVerticeInTheMiddle;

    }

    void addChar(u8 newSimbol, u32 indexSimbol) {
        for(Vertice* parentNewLeaf = nullptr; parentNewLeaf != start;) {
            Position newPos = tryGoDownInTree(lastNotLeaf, indexSimbol, indexSimbol + 1);
            if (newPos.vertice) {
                lastNotLeaf = newPos;
                break;
            }
            parentNewLeaf = split(lastNotLeaf);
            auto newLeaf = new Vertice(indexSimbol, sequence.size(), parentNewLeaf);
            lastNotLeaf.vertice = getSuff(parentNewLeaf);
            parentNewLeaf->getChild(newSimbol) = newLeaf;
            lastNotLeaf.distance = lastNotLeaf.vertice->length();
        }
    }


public:
    virtual void construct() override {
        start = new Vertice();
        lastNotLeaf = Position(start, 0);
        for (u32 i = 0; i < sequence.size(); ++i) {
            addChar(sequence[i], i);
        }
    }
    ~SuffixTree() {
        delete start;
    }

    void findLenAns(Vertice* v) {
        for(const auto& it: v->children) {
            findLenAns(it.second);
            v->count += it.second->count;
        }
        if (v->count == 0)
            v->count = 1;
        if(v->count * v->length() > answer.answer) {
            answer.answer = v->count * v->length();
            answer.length = v->length();
        }
    }

    void findString(Vertice* v, u32 le) {
        u32 commonLength = le + v->length();
        if ((commonLength * v->count) == answer.answer) {
            answerWasFound = true;
            return;
        }
        for(const auto& it: v->children) {
            if (le + it.second->length() > answer.length)
                continue;
            for (u32 i = it.second->edgeStart; i < it.second->edgeFinish; ++i) {
                answer.string.push_back(it.first);
            }
            findString(it.second, commonLength);
            if(answerWasFound)
                return;
            for (u32 i = it.second->edgeStart; i < it.second->edgeFinish; ++i) {
                answer.string.pop_back();
            }
        }
    }

    virtual AnswerType getAnswer() override {
        answer.string.reserve(sequence.size());
        findLenAns(start);
        findString(start, 0);
        return answer;
    }
};

template<typename AlgoType = SuffixTree>
void findRefren(const vector<u8> &array) {
    AlgoType au;
    au.run(array);
}


int main() {
    u32 n;
    u32 m;
    cin >> n >> m;
    vector<u8> array(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> array[i];
    }
    findRefren(array);
    return 0;
}