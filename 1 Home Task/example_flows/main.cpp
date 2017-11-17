#include <iostream>
#include <vector>

typedef uint32_t u32;
typedef long long TFlow;

using std::vector;

struct Edge {
    u32 start, finish;
    TFlow capacity, flow;
    Edge(u32 s, u32 f, TFlow c) : start(s), finish(f), capacity(c) {}
};

class Network {
    vector<Edge> edges_;
    vector<int> start_, previous_;
    u32 verticesNumber_, source_, sink_;

    void addEdgeLocal(u32 start, u32 finish, TFlow capacity) {
        Edge newEdge(start, finish, capacity);
        edges_.push_back(newEdge);
    }
public:
    Network(u32 n, u32 s, u32 t) : verticesNumber_(n), source_(s), sink_(t) {}

    void addEdge(u32 s, u32 f, TFlow capacity) {
        addEdgeLocal(s, f, capacity);
        addEdgeLocal(f, s, 0);
    }
    friend class iterator;
    class iterator : public std::iterator<std::forward_iterator_tag, Edge> {
        Network network;
        int edgeNumber;
    public:
        iterator(const Network& n, int e) : network(n), edgeNumber(e) {}
        void next() {
            edgeNumber = network.previous_[edgeNumber];
        }
        iterator& operator++() {
            next();
            return *this;
        }
        bool isValid() const {
            return edgeNumber >= 0;
        }
        const Edge& operator*() const {
            return network.edges_[edgeNumber];
        }
        Edge* operator->() const {
            return &(network.edges_[edgeNumber]);
        }
        TFlow getCapasity() const {
            return network.edges_[edgeNumber].capacity;
        }
        void pushFlow(TFlow newFlow) {
            network.edges_[edgeNumber].flow += newFlow;
        }
    };

    iterator& begin(u32 v) const {
        iterator newIt(*this, start_[v]);
        return newIt;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}