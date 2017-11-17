#include <iostream>
#include <vector>

using std::vector;

vector<vector<uint32_t>> graph;
vector<uint32_t> used, pr;
uint32_t n, m, currentIteration, ans;

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
    used.assign(n, 0);
    currentIteration = 0, ans = 0;
    pr.assign(m, -1);
    for (uint32_t v = 0; v < n; ++v) {
        if (dfs(v)) {
            ++ans;
            ++currentIteration;
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}