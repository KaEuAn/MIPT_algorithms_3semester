#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>
using ld = long double;
using std::cin;
using std::cout;
using std::vector;

vector<vector<uint32_t>> graph;
vector<vector<uint32_t>> newgraph;
vector<uint32_t> used, pr;
uint32_t n, m,  currentIteration;

vector<uint32_t> usedl;

bool dfs(uint32_t v) {
    if (used[v] == currentIteration)
        return false;
    used[v] = currentIteration;
    for (uint32_t u : graph[v]) {
        if (pr[u] == -1 || dfs(pr[u])) {
            pr[u] = v;
            return true;
        }
    }
    return false;
}

void findMaximalMatching() {
    used.assign(m, 0);
    currentIteration = 1;
    pr.assign(n, -1);
    for (uint32_t v = 0; v < m; ++v) {
        if (dfs(v)) {
            ++currentIteration;
        }
    }
}

void minpokr(int i) {
    if (usedl[i] == 1)
        return;
    usedl[i] = 1;
    for (int j = 0; j < newgraph[i].size(); ++j) {
        minpokr(newgraph[i][j]);
    }
}

int main() {
    int k;
    cin >> k;
    for (int iii = 0; iii < k; ++iii) {
        cin >> m >> n;
        graph.assign(m, vector<uint32_t>());
        uint32_t sp[m][n];
        for (uint32_t i = 0; i < m; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                sp[i][j] = 1;
            }
        }
        for (uint32_t i = 0; i < m; ++i) {
            uint32_t a;
            cin >> a;
            while(a != 0) {
                sp[i][a - 1] = 0;
                cin >> a;
            }
        }
        for (uint32_t i = 0; i < m; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                if(sp[i][j] == 1) {
                    graph[i].push_back(j);
                }
            }
        }
        findMaximalMatching();
        vector<uint32_t> isVisitedL;
        isVisitedL.assign(m, 0);
        for (int i = 0; i < n; ++i) {
            if(pr[i] != -1)
                isVisitedL[pr[i]] = 1;
        }
        
        newgraph.assign(n + m, vector<uint32_t>());
        for (uint32_t i = 0; i < m; ++i) {
            for (uint32_t j = 0; j < graph[i].size(); ++j) {
                if(pr[graph[i][j]] != i) {
                    newgraph[i].push_back(m + graph[i][j]);
                }else {
                    newgraph[m + graph[i][j]].push_back(i);
                }
            }
        }
        usedl.assign(m + n, 0);
        for (int i = 0; i < m; ++i) {
            if(usedl[i] == 0 && ! isVisitedL[i])
                minpokr(i);
        }
        uint32_t couL = m;
        uint32_t couR = n;
        for (int i = 0; i < m; ++i) {
            if (usedl[i] == 0) {
                --couL;
            }
        }
        for (int i = 0; i < n; ++i) {
            if (usedl[i + m] == 1) {
                --couR;
            }
        }
        cout << couL + couR << '\n';
        cout << couL << ' ' << couR << '\n';
        for (int i = 0; i < m; ++i) {
            if (usedl[i] == 1) {
                cout << i + 1 << ' ';
            }
        }
        cout << '\n';
        for (int i = 0; i < n; ++i) {
            if (usedl[i + m] == 0) {
                cout << i + 1 << ' ';
            }
        }
        cout << '\n';
    }
    return 0;
}