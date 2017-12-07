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
using u32 = uint32_t;
using u8 = uint8_t;
using std::shared_ptr;
using std::pair;

class AnswerType {
protected:
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

public:
    SolveTask() {};
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
    AnswerType answer;
    bool answerWasFound;

    void addChar(u8 newSimbol) {
        auto newLast = std::make_shared<Vertice>(lastString->len + 1);
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
                auto clone = std::make_shared<Vertice>(itChild, it->len + 1);
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
    SuffixAutomat(): answerWasFound(false) {}
    virtual void construct() override {
        sortedByLength.assign(sequence.size() + 1, vector<Vertice*>());
        start = lastString = std::make_shared<Vertice>();
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
    static const int INF = 999999999;
    struct Vertice {
        u32 len;
        Vertice *suff;
        u32 suffNumber;
        u32 count;
        map<u8, shared_ptr<Vertice>> children;
    };
    struct position {
        Vertice* vertice;
        u32 dist;
    };


public:

};

template<typename AlgoType = SuffixAutomat>
void fineRefren(const vector<u8>& array) {
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
    fineRefren(array);
    return 0;
}