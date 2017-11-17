#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <exception>
#include <stdexcept>

typedef uint32_t u32;
typedef long long TFlow;

using std::vector;
using std::queue;
using std::cin;
using std::cout;

struct Edge {
    u32 start, finish;
    TFlow capacity;
    mutable TFlow flow;

    Edge(u32 s, u32 f, TFlow c) : start(s), finish(f), capacity(c), flow(0) {}

    TFlow fcapacity() const {
        return capacity - flow;
    }
};

struct InnerEdge : Edge {
    u32 reverse;
    u32 number;

    InnerEdge(u32 s, u32 f, TFlow c, u32 rev, u32 numb = -1) :
            Edge(s, f, c), reverse(rev), number(numb) {}

};


struct MaxFlowAnswer {
    u32 maxFlow;
};

class Net {
    vector<vector<InnerEdge> > graph_;
    u32 source_;
    u32 sink_;
    u32 verticesCount_;
    u32 edgesCount_;
    static const unsigned long long INFedge = 2000000;

public:
    Net() {}

    Net(u32 verticeCount, u32 source, u32 sink) :
            source_(source), sink_(sink), verticesCount_(verticeCount), edgesCount_(0) {
        graph_.resize(verticeCount);
    }

    void pushEdge(u32 start, u32 finish, TFlow capacity = INFedge) {
        ++edgesCount_;
        Edge edge(start, finish, capacity);
        graph_[start].push_back(
                InnerEdge(start, finish, capacity, graph_[edge.finish].size(), edgesCount_));
        graph_[finish].push_back(InnerEdge(finish, start, 0, graph_[start].size() - 1));
    }

    MaxFlowAnswer returnFlowAnswer() {
        MaxFlowAnswer ans;
        ans.maxFlow = 0;
        for (const InnerEdge &edge: graph_[source_]) {
            ans.maxFlow += edge.flow;
        }

        return ans;
    }

    InnerEdge &reverseEdge(const InnerEdge &edge) const {
        return const_cast<InnerEdge &>(graph_[edge.finish][edge.reverse]);
    }

    u32 getSink() const {
        return sink_;
    }
    u32 getSource() const {
        return source_;
    }
    u32 getVerticesCount() const {
        return verticesCount_;
    }
    u32 getEdgesCount() const {
        return edgesCount_;
    }
    const vector<vector<InnerEdge>>& getGraph() const{
        return graph_;
    }

};

class MaxFlowFinder {
protected:
    Net net;
    const u32 INF = 900000000;

    virtual void construct(u32 verticesCount) = 0;


public:
    virtual ~MaxFlowFinder() {}

    virtual void findMaxFlow() = 0;

    MaxFlowAnswer run(u32 verticesCount, const Net& externalNet, u32 source, u32 sink) {
        net = externalNet;
        construct(verticesCount);
        findMaxFlow();
        return net.returnFlowAnswer();
    }
};


class PrePushFlow : public MaxFlowFinder {
    std::vector<std::vector<InnerEdge>::const_iterator> currentEdge;
    vector<u32> height;
    vector<u32> excess;

    void pushFlowAlongEdge(const InnerEdge &edge, TFlow flow) {
        edge.flow += flow;
        net.reverseEdge(edge).flow -= flow;
        excess[edge.start] -= flow;
        excess[edge.finish] += flow;
    }

public:
    virtual ~PrePushFlow() {}

    PrePushFlow() {}

    virtual void construct(u32 verticesCount) override {
        height.assign(verticesCount, 0);
        height[net.getSource()] = net.getVerticesCount();
        excess.assign(verticesCount, 0);
        currentEdge.resize(verticesCount);
        for (u32 i = 0; i < verticesCount; ++i) {
            currentEdge[i] = net.getGraph()[i].begin();
        }
    }

    void push(const InnerEdge &edge) {
        TFlow push = std::min(edge.fcapacity(), static_cast<TFlow>(excess[edge.start]));
        pushFlowAlongEdge(edge, push);
    }

    void relabel(u32 verticeNumber) {
        u32 currentHeight = INF;
        for (const InnerEdge &edge: net.getGraph()[verticeNumber]) {
            if (edge.fcapacity() > 0)
                currentHeight = std::min(currentHeight, height[edge.finish]);
        }
        height[verticeNumber] = currentHeight + 1;
    }

    void discharge(u32 verticeNumber) {
        while (excess[verticeNumber] > 0) {
            if (currentEdge[verticeNumber] == net.getGraph()[verticeNumber].end()) {
                relabel(verticeNumber);
                currentEdge[verticeNumber] = net.getGraph()[verticeNumber].begin();
            } else {
                const InnerEdge &edge = *currentEdge[verticeNumber];
                if (edge.fcapacity() > 0 && height[edge.finish] + 1 == height[edge.start]) {
                    push(edge);
                } else {
                    ++currentEdge[verticeNumber];
                }
            }
        }
    }

    virtual void findMaxFlow() override {
        u32 excessCount = 0;
        for (const InnerEdge &edge: net.getGraph()[net.getSource()]) {
            pushFlowAlongEdge(edge, edge.capacity);
            if (excess[edge.finish] && edge.capacity > 0) {
                ++excessCount;
            }
        }
        excess[net.getSource()] = 0;
        while (excessCount > 0) {
            for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
                if (verticeNumber != net.getSource() && verticeNumber != net.getSink())
                    discharge(verticeNumber);
            }
            excessCount = 0;
            for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
                if (verticeNumber != net.getSource() && verticeNumber != net.getSink() && excess[verticeNumber] > 0) {
                    ++excessCount;
                    break;
                }
            }
        }
    }
};

class MKM : public MaxFlowFinder {
    enum directions {
        FORWARD,
        BACKWARD
    };
    vector<u32> potentials[2];
    vector<u32> blocked;
    vector<u32> layerNumber;
    vector<vector<InnerEdge>::const_iterator> currentEdge[2];

    u32 getVertexPotential(u32 verticeNumber) const {
        if (blocked[verticeNumber])
            return INF;
        return std::min(potentials[FORWARD][verticeNumber], potentials[BACKWARD][verticeNumber]);
    }

    bool makeLayers() {
        std::queue<u32> layerQueue;
        layerQueue.push(net.getSource());
        layerNumber.assign(net.getVerticesCount(), INF);
        layerNumber[net.getSource()] = 0;
        while (!layerQueue.empty()) {
            u32 verticeNumber = layerQueue.front();
            layerQueue.pop();
            for (const Edge &edge: net.getGraph()[verticeNumber]) {
                if (edge.fcapacity() > 0 && layerNumber[edge.finish] > layerNumber[edge.start] + 1) {
                    layerNumber[edge.finish] = layerNumber[edge.start] + 1;
                    layerQueue.push(edge.finish);
                }
            }
        }
        return layerNumber[net.getSink()] != INF;
    }

    void push(u32 verticeNumber, const InnerEdge &edge, TFlow flow) {
        edge.flow += flow;
        net.reverseEdge(edge).flow -= flow;
        potentials[FORWARD][verticeNumber] -= flow;
        potentials[BACKWARD][edge.finish] -= flow;
    }

    void makePotentials() {
        blocked.assign(net.getVerticesCount(), 0);
        for (u32 i = 0; i < 2; ++i) {
            potentials[i].assign(net.getVerticesCount(), 0);
            currentEdge[i].resize(net.getVerticesCount());
            for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
                currentEdge[i][verticeNumber] = net.getGraph()[verticeNumber].begin();
            }
        }

        for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
            if (layerNumber[verticeNumber] >= layerNumber[net.getSink()] && verticeNumber != net.getSink()) {
                blocked[verticeNumber] = 1;
            }
        }

        potentials[FORWARD][net.getSink()] = INF;
        potentials[BACKWARD][net.getSource()] = INF;
        for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
            for (const InnerEdge &edge: net.getGraph()[verticeNumber]) {
                if (edge.fcapacity() > 0 && layerNumber[edge.finish] == layerNumber[verticeNumber] + 1 &&
                    !blocked[edge.finish]) {
                    potentials[FORWARD][verticeNumber] += edge.fcapacity();
                    potentials[BACKWARD][edge.finish] += edge.fcapacity();
                }
            }
        }
    }

    void onePush(u32 verticeNumber, u32 direction, TFlow flow) {
        if (flow == 0)
            return;
        vector<TFlow> pushedValue(net.getVerticesCount(), 0);
        std::queue<u32> goIn;
        goIn.push(verticeNumber);
        pushedValue[verticeNumber] = flow;
        while (!goIn.empty()) {
            u32 currentVerticeNumber = goIn.front();
            if (currentVerticeNumber == net.getSource() || currentVerticeNumber == net.getSink())
                break;
            goIn.pop();
            for (; pushedValue[currentVerticeNumber] > 0 &&
                   (currentEdge[direction][currentVerticeNumber] != net.getGraph()[verticeNumber].end());
                   ++currentEdge[direction][currentVerticeNumber]) {
                const InnerEdge &edge = *currentEdge[direction][currentVerticeNumber];
                if (!blocked[edge.finish] && layerNumber[edge.start] == layerNumber[edge.finish] + 2 * direction - 1) {
                    u32 ver = currentVerticeNumber;
                    const InnerEdge *newedge = &edge;
                    if (direction == BACKWARD) {
                        newedge = &net.reverseEdge(edge);
                    }
                    const InnerEdge &nnedge = *newedge;
                    if (nnedge.fcapacity() > 0) {
                        u32 ans = std::min(nnedge.fcapacity(), pushedValue[ver]);
                        push(ver, nnedge, ans);
                        pushedValue[edge.finish] += ans;
                        pushedValue[currentVerticeNumber] -= ans;
                        goIn.push(edge.finish);
                        if (pushedValue[currentVerticeNumber] == 0)
                            break;
                    }

                }
            }
        }
    }

    void remakePotentials(const InnerEdge &edge, u32 minFi) {
        if (layerNumber[edge.finish] == layerNumber[minFi] + 1) {
            potentials[BACKWARD][edge.finish] -= edge.fcapacity();
        }
        if (layerNumber[edge.finish] + 1 == layerNumber[minFi]) {
            potentials[FORWARD][edge.finish] -= net.reverseEdge(edge).fcapacity();
        }
    }

public:
    MKM() {}

    virtual ~MKM() {}

    virtual void findMaxFlow() override {
        while (makeLayers()) {
            makePotentials();
            bool allIsBlocked = false;
            while (!allIsBlocked) {
                u32 minPotential = 0;
                for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber) {
                    if (getVertexPotential(minPotential) > getVertexPotential(verticeNumber)) {
                        minPotential = verticeNumber;
                    }
                }
                u32 value = getVertexPotential(minPotential);
                onePush(minPotential, FORWARD, value);
                onePush(minPotential, BACKWARD, value);
                blocked[minPotential] = 1;
                for (const InnerEdge &edge: net.getGraph()[minPotential]) {
                    if (!blocked[edge.finish]) {
                        remakePotentials(edge, minPotential);
                    }
                }
                allIsBlocked = true;
                for (u32 verticeNumber = 0; verticeNumber < net.getVerticesCount(); ++verticeNumber)
                    if (!blocked[verticeNumber]) {
                        allIsBlocked = false;
                        break;
                    }
            }
        }
    }

    virtual void construct(u32 verticesCount) override {
        potentials[FORWARD].assign(verticesCount, 0);
        potentials[BACKWARD].assign(verticesCount, 0);
        potentials[FORWARD][net.getSink()] = INF;
        potentials[BACKWARD][net.getSource()] = INF;
        blocked.assign(net.getVerticesCount(), 0);
    }
};

template<typename T = PrePushFlow>
TFlow find(Net& net, u32 n) {
    T pushFlow;
    return pushFlow.run(n + 2, net, n, n + 1).maxFlow;
}

void readAndCalculate() {
    u32 n;
    cin >> n;
    vector<TFlow> x;
    x.assign(n + 4, 0);
    Net net(n + 2, n, n + 1);
    std::vector<Edge> edges;
    edges.reserve(n * n);
    u32 preAns = 0;
    for (u32 i = 0; i < n; ++i) {
        cin >> x[i];
        if (x[i] >= 0) {
            net.pushEdge(n, i, x[i]);
            preAns += x[i];
        }
        else
            net.pushEdge(i, n + 1, -x[i]);
    }
    for (u32 i = 0; i < n; ++i) {
        int ak;
        cin >> ak;
        for (u32 j = 0; j < ak; ++j) {
            u32 aj;
            cin >> aj;
            net.pushEdge(i, aj - 1);
        }
    }
    TFlow maxFlow1 = find(net, n);
    TFlow maxFlow2 = find<MKM>(net, n);
    if (maxFlow1 == maxFlow2)
        cout << preAns - maxFlow1;
    else
        throw std::bad_exception();
}

int main() {
    readAndCalculate();
    return 0;
}