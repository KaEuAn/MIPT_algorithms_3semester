#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>

typedef uint32_t u32;
typedef long long TFlow;
const long long INF = 199000000;

using std::vector;
using std::string;
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
    uint32_t ans;
    vector<u32> used;
    string af;
    string bf;
    u32 n, m;
    bool reversed;

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
    Network(string a, string b) {
        reversed = false;
        if (a.length() < b.length()) {
            std::swap(a, b);
            reversed = true;
        }
        af = a;
        bf = b;
        n = a.length();
        m = b.length();
        source_ = n + m;
        sink_ = n + m + 1;
        ans = 0;
        verticesNumber_ = n + m + 2;
        start_ = new int[verticesNumber_];
        newStart_ = new int[verticesNumber_];
        for (int i = 0; i < verticesNumber_; ++i) {
            start_[i] = -1;
            newStart_[i] = -1;
        }
        for (uint32_t i = 0; i < n - m + 1; ++i) {
            for (uint32_t j = 0; j < m; ++j) {
                if((a[i + j] == '1' && b[j] == '0') || (a[i + j] == '0' && b[j] == '1'))
                    ++ans;
                else if(a[i + j] == '1' && b[j] == '?') {
                    addEdgeLocal(source_, n + j, 1);
                    addEdgeLocal(n + j, source_, 0);
                } else if(a[i + j] == '0' && b[j] == '?') {
                    addEdgeLocal(j + n, sink_, 1);
                    addEdgeLocal(sink_, j + n, 0);
                } else if(a[i + j] == '?' && b[j] == '1') {
                    addEdgeLocal(source_, i + j, 1);
                    addEdgeLocal(i + j, source_, 0);
                } else if(a[i + j] == '?' && b[j] == '0') {
                    addEdgeLocal(i + j, sink_, 1);
                    addEdgeLocal(sink_, i + j, 0);
                } else if(a[i + j] == '?' && b[j] == '?') {
                    addEdgeLocal(i + j, j + n, 1);
                    addEdgeLocal(j + n, i + j, 1);
                }
            }
        }
    }
    ~Network() {
        delete[] start_;
        delete[] newStart_;
    }

    void addEdge(u32 s, u32 f, TFlow capacity) {
        addEdgeLocal(s, f, capacity);
        addEdgeLocal(f, s, capacity);
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
    void calculateMaxFlow() {
        TFlow flow = 0;
        while(makeLayered()) {
            while(TFlow pushed = pushFlow(source_, INF))
                flow += pushed;
            clearStartAndPrevious();
        }
        cout << flow + ans;
        used.assign(verticesNumber_, 0);
        dfsRestore(source_);
        for (int i = 0; i < n; ++i) {
            if(af[i] == '?')
                af[i] = '0';
        }
        for (int i = 0; i < m; ++i) {
            if(bf[i] == '?')
                bf[i] = '0';
        }
        if(reversed)
            std::swap(af, bf);
        cout << '\n' << af << '\n';
        cout << bf;
    }

    void dfsRestore(u32 i) {
        if(used[i] == 1) return;
        used[i] = 1;
        if(i < n + m) {
            if(i >= n) {
                bf[i - n] = '1';
            } else {
                af[i] = '1';
            }
        }
        for(int edgeNumber = start_[i]; edgeNumber >= 0; edgeNumber = previous_[edgeNumber]) {
            if(edges_[edgeNumber].capacity - edges_[edgeNumber].flow > 0)
                dfsRestore(edges_[edgeNumber].finish);
        }
    }
};



int main() {
    u32 n, m;
    string a;
    string b;
    cin >> a >> b;
    Network myNet(a, b);
    myNet.calculateMaxFlow();
    return 0;
}