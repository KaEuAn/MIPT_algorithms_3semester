#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

typedef uint32_t u32;
typedef long long TFlow;
const long long INF = 199000000;

using std::vector;
using std::queue;
using std::cin;
using std::cout;
using ld = long double;

struct Edge {
    u32 start, finish;
    TFlow capacity, flow;
    ld cost;
    u32 opposite;
    u32 number;
    Edge(u32 s, u32 f, TFlow c, ld co, u32 op, u32 nu) :
            start(s), finish(f), capacity(c), flow(0), cost(co), opposite(op), number(nu), isBlocked(false) {}

    TFlow getFc() const {
        return capacity - flow;
    }
};

class Network {
    u32 verticesNumber_, source_, sink_;
    u32 maxAnswer;
    vector<ld> dei;
    vector<vector<Edge>> sp;
    vector<u32> pred;
    vector<u32> predEdge;
    u32 flow;
    u32 cost;
    vector<vector<u32>> answer;

    void addEdgeLocal(u32 start, u32 finish, TFlow capacity, TFlow cost, u32 op, u32 number) {
        Edge newEdge(start, finish, capacity, cost, op, number);
        sp[start].push_back(newEdge);
    }


public:
    Network(u32 n, u32 s, u32 t, u32 c, u32 k) : verticesNumber_(n), source_(s), sink_(t), maxAnswer(k), flow(0), cost(0) {
        sp.assign(verticesNumber_, vector<Edge>());
        answer.assign(k, vector<u32>());
        for (u32 i = 0; i < c; ++i) {
            u32 a1, a2, a3;
            cin >> a1 >> a2 >> a3;
            --a1; --a2;
            addEdge(a1, a2, 1, a3, i + 1);
            addEdge(a2, a1, 1, a3, i + 1);
        }
    }

    void addEdge(u32 s, u32 f, TFlow capacity, TFlow cost, u32 number) {
        addEdgeLocal(s, f, capacity, cost, sp[f].size(), number);
        addEdgeLocal(f, s, 0, -cost, sp[s].size(), number);
    }
    void Deik() {
        dei.assign(verticesNumber_, INF);
        pred.assign(verticesNumber_, 0);
        predEdge.assign(verticesNumber_, 0);
        vector<int> visited(verticesNumber_, 0);
        dei[source_] = 0;
        for (int i = 0; i < verticesNumber_; ++i) {
            u32 ma = -1;
            for (u32 j = 0; j < verticesNumber_; ++j) {
                if (! visited[j] && ( ma == -1 || dei[j] < dei[ma])) {
                    ma = j;
                }
            }
            if (dei[ma] == INF)
                break;
            visited[ma] = true;
            for (u32 j = 0; j < sp[ma].size(); ++j) {
                u32 fin = sp[ma][j].finish;
                ld cost = sp[ma][j].cost;
                if (sp[ma][j].getFc() != 0 && dei[ma] + cost < dei[fin]) {
                    dei[fin] = dei[ma] + cost;
                    pred[fin] = ma;
                    predEdge[fin] = j;
                }
            }
        }

    }

    void run() {
        while(flow < maxAnswer) {
            Deik();
            if(dei[sink_] == INF)
                break;
            for (int ver = sink_; ver != source_; ver = pred[ver]) {
                u32 first = pred[ver];
                u32 edge = predEdge[ver];
                u32 revedge = sp[first][edge].opposite;
                ++sp[first][edge].flow;
                --sp[ver][revedge].flow;
                cost += sp[first][edge].cost;
                answer[flow].push_back(sp[first][edge].number);
            }
            ++flow;
        }
        if (flow < maxAnswer)
            cout << -1;
        else {
            cout << std::setprecision(10) << static_cast<ld>(cost) / (maxAnswer) << '\n';
            printAnswer();
        }
    }
    void printAnswer() {
        for (int i = 0; i < maxAnswer; ++i) {
            cout << answer[i].size() << ' ';
            for (int j = answer[i].size() - 1; j >= 0; --j) {
                cout << answer[i][j] << ' ';
            }
            cout << '\n';
        }
    }

};

int main() {
    u32 n, m, k;
    cin >> n >> m >> k;
    cout << std::setprecision(10);
    Network network(n, 0, n - 1, m, k);
    network.run();
    return 0;
}