//
// Created by Eugene on 10.10.2017.
//

#ifndef MAIN_FLOWS_FIRST_TRY_H
#define MAIN_FLOWS_FIRST_TRY_H

#endif //MAIN_FLOWS_FIRST_TRY_H
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

    TFlow fcapacity() const {
        return capacity - flow;
    }
};

struct InnerEdge: Edge{
    u32 reverse;
    u32 number;
    InnerEdge(u32 s, u32 f, TFlow c, u32 rev, u32 numb = -1) :
            Edge(s, f, c), reverse(rev), number(numb) {}

};

struct Vertice {
    u32 excess;
    vector<u32> outgoingEdges;
    u32 height;
    u32 currentEdge;
    explicit Vertice(): excess(0), height(0), currentEdge(0) {}
};



class PrePushFlow {
    vector<Vertice> vertices_;
    vector<Edge> edges_;
    u32 verticesNumber;
    u32 source;

    u32 neighboringEdge(u32 edgeNumber) const {
        return edgeNumber ^ 1;
    }
    void pushFlowAlongEdge(u32 edgeNumber, TFlow flow) {
        Edge edge = edges_[edgeNumber];
        edge.flow += flow;
        edges_[neighboringEdge(edgeNumber)].flow -= flow;
        vertices_[edge.start].excess -= flow;
        vertices_[edge.finish].excess -= flow;
    }
    void firstPush() {
        for (u32 edgeNumber: vertices_[source].outgoingEdges) {
            pushFlowAlongEdge(edgeNumber, edges_[edgeNumber].capacity);
        }
    }
    void addEdgeLocal(u32 s, u32 f, u32 c) {
        edges_.push_back(Edge(s, f, c));
        vertices_[s].outgoingEdges.push_back(edges_.size() - 1);
    }

    void addEdge(u32 s, u32 f, u32 c) {
        addEdgeLocal(s, f, c);
        addEdgeLocal(f, s, 0);
    }
    void clear() {
        edges_.clear();
        vertices_.clear();
    }

public:
    PrePushFlow(u32 vN, u32 s): verticesNumber(vN), source(s) {
        vertices_.assign(verticesNumber, Vertice());
        //some init for edges
    }

    void push(u32 edgeNumber) {
        TFlow push = std::min(edges_[edgeNumber].fcapacity(), vertices_[edges_[edgeNumber].start].excess);
        pushFlowAlongEdge(edgeNumber, push);
    }

    void relabel(u32 verNumb) {
        u32 currentHeight = INF;
        for (u32 edNumb: vertices_[verNumb].outgoingEdges) {
            if(edges_[edNumb].fcapacity() > 0)
                currentHeight = std::min(currentHeight, vertices_[edges_[edNumb].finish].height);
        }
        vertices_[verNumb].height = currentHeight + 1;
    }

    void discharge(u32 verNumb) {
        while(vertices_[verNumb].excess > 0) {
            if() {

            }else {
                if() {

                }else {

                }
            }
        }
    }

    void findMaxFlow() {
        vertices_[source].height = verticesNumber;
    }
};
