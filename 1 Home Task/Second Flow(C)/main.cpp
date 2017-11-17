#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

typedef uint32_t u32;
typedef long long TFlow;
const long long INF = 199000000;

using std::vector;
using std::queue;
using std::cin;
using std::cout;

struct Edge {
    u32 start, finish;
    TFlow capacity, flow;
    Edge(u32 s, u32 f, TFlow c) : start(s), finish(f), capacity(c), flow(0) {}
};

class Network {
    vector<Edge> edges_;
    int* start_;
    vector<int> previous_;
    int* newStart_;
    vector<int> newPrevious_;
    u32 verticesNumber_, source_, sink_;

    void addEdgeLocal(u32 start, u32 finish, TFlow capacity) {
        Edge newEdge(start, finish, capacity);
        edges_.push_back(newEdge);
        previous_.push_back(start_[start]);
        newPrevious_.push_back(-1);
        start_[start] = previous_.size() - 1;
    }
    void newAdd(u32 start, int edgeNumber) {
        newPrevious_[edgeNumber] = (newStart_[start]);
        newStart_[start] = edgeNumber;
    }
    void clearStartAndPrevious() {
        for (int i = 0; i < verticesNumber_; ++i) {
            newStart_[i] = -1;
        }
        for(int i = 0; i < newPrevious_.size(); ++i){
            newPrevious_[i] = -1;
        }
    }

public:
    Network(u32 n, u32 s, u32 t, u32 c) : verticesNumber_(n), source_(s), sink_(t) {
        start_ = new int[verticesNumber_];
        newStart_ = new int[verticesNumber_];
        for (int i = 0; i < verticesNumber_; ++i) {
            start_[i] = -1;
            newStart_[i] = -1;
        }
        for (int i = 0; i < c; ++i) {
            u32 a1, a2, a3;
            cin >> a1 >> a2 >> a3;
            --a1; --a2;
            addEdge(a1, a2, a3);
        }
    }
    ~Network() {
        delete[] start_;
        delete[] newStart_;
    }

    void addEdge(u32 s, u32 f, TFlow capacity) {
        addEdgeLocal(s, f, capacity);
        addEdgeLocal(f, s, 0);
    }
    friend class iterator;
    class iterator : public std::iterator<std::forward_iterator_tag, Edge> {
        Network* network;
    public:
        int edgeNumber;
        iterator(Network* n, int e) : network(n), edgeNumber(e) {}
        void next() {
            edgeNumber = network->previous_[edgeNumber];
        }
        iterator& operator++() {
            next();
            return *this;
        }
        void makeTheEnd() {
            network->previous_[edgeNumber] = -1;
        }
        bool isValid() const {
            return edgeNumber >= 0;
        }
        const Edge& operator*() const {
            return network->edges_[edgeNumber];
        }
        Edge* operator->() const {
            return &(network->edges_[edgeNumber]);
        }
        TFlow getCapacity() const {
            return network->edges_[edgeNumber].capacity;
        }
        TFlow getFlow() const {
            return network->edges_[edgeNumber].flow;
        }
        TFlow getFCapacity() const {
            return getCapacity() - getFlow();
        }
        int neighbor() const {
            return edgeNumber ^ 1;
        }
        void pushFlow(TFlow newFlow) {
            network->edges_[edgeNumber].flow += newFlow;
            network->edges_[neighbor()].flow -= newFlow;
        }
    };

    iterator begin(u32 v) {
        return iterator(this, start_[v]);;
    }
    bool makeLayered() {
        int* isVisited = new int[verticesNumber_];
        for (int i = 0; i < verticesNumber_; ++i) {
            isVisited[i] = -1;
        }
        isVisited[source_] = 0;
        queue<u32> BFSVertices;
        BFSVertices.push(source_);
        while(! BFSVertices.empty()) {
            u32 vnow = BFSVertices.front();
            BFSVertices.pop();
            for (iterator it = begin(vnow); it.isValid(); ++it) {
                if(it.getFCapacity() > 0 && (isVisited[it->finish] == -1 ||
                        isVisited[it->finish] == isVisited[it->start] + 1)) {
                    newAdd(vnow, it.edgeNumber);
                    if (isVisited[it->finish] == -1) {
                        BFSVertices.push(it->finish);
                        isVisited[it->finish] = isVisited[it->start] + 1;
                    }
                }
            }
        }
        bool ans = isVisited[sink_] != -1;
        delete[] isVisited;
        return ans;
    }
    TFlow pushFlow(u32 v, TFlow flow) {
        if(flow <= 0) return 0;
        if(v == sink_) return flow;
        for(int edgeNumber = newStart_[v]; edgeNumber >= 0; edgeNumber = newPrevious_[edgeNumber]) {
            TFlow pushed = pushFlow(edges_[edgeNumber].finish,
                                    std::min(flow, edges_[edgeNumber].capacity - edges_[edgeNumber].flow));
            if(pushed) {
                edges_[edgeNumber].flow += pushed;
                edges_[edgeNumber^1].flow -= pushed;
                return pushed;
            }
            newStart_[v] = newPrevious_[edgeNumber];

        }
        newStart_[v] = -1;
        return 0;

    }
    TFlow calculateMaxFlow() {
        TFlow flow = 0;
        while(makeLayered()) {
            while(TFlow pushed = pushFlow(source_, INF))
                flow += pushed;
            clearStartAndPrevious();
        }
        clearStartAndPrevious();
        makeZeroFlow();
        return flow;
    }
    void makeMoreOn(u32 start, u32 finish, int sum) {
        for (int x = start_[start]; x >= 0 ; x = previous_[x]) {
            if (edges_[x].finish == finish && x % 2 == 0) {
                edges_[x].capacity += sum;
                return;
            }
        }
        ++verticesNumber_;
        addEdge(start, finish, sum);
    }
    void makeZeroFlow() {
        for (int i = 0; i < edges_.size(); ++i) {
            edges_[i].flow = 0;
        }
    }
};


int main() {
    using std::cin; using std::cout;
    u32 n, m, t;
    cin >> n >> m;
    Network network(n, 0, n-1, m);
    cout << network.calculateMaxFlow() << '\n';
    cin >> t;
    for (int i = 0; i < t; ++i) {
        u32 c, a, b;
        cin >> c >> a >> b;
        --a; --b;
        if(c == 2) {
            network.makeMoreOn(a, b, -1);
        } else {
            network.makeMoreOn(a, b, 1);
        }
        cout << network.calculateMaxFlow() << '\n';
    }
    return 0;
}