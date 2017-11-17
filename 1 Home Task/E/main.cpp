#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
const int INF = 99999999;

vector<vector<uint32_t>> graph;
vector<uint32_t> used, pr, newused;
uint32_t n1, m1, currentIteration, ans;
uint32_t m, n, a, b, count_pl = 0;
bool sp[301][301];

uint32_t code(uint32_t i, uint32_t j) {
    return i * m + j;
}

std::pair<uint32_t , uint32_t> decode(uint32_t a){
    return std::pair<uint32_t , uint32_t>(a/m, a%m);
};

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
    for (uint32_t v = 0; v < n * m; ++v) {
        auto a = decode(v);
        bool afir = (a.first + a.second) % 2 == 0 && sp[a.first][a.second] && newused[v] == 0;
        if (afir && dfs(v)) {
            ++ans;
            ++currentIteration;
        }
    }
}
void maybe_add (uint32_t i, uint32_t j) {
    auto x = decode(i);
    auto y = decode(j);
    if ((x.first + x.second) % 2 == 0 && pr[j] == -1  && newused[i] == 0) {
        newused[i] = 1;
        pr[j] = i;
        ++ans;
    } else if ((y.first + y.second) % 2 == 0 &&  pr[i] == -1 && newused[j] == 0) {
        pr[i] = j;
        newused[j] = 1;
        ++ans;
    }
}


int main() {
    using std::cout;
    using std::cin;
    cin >> n >> m >> a >> b;
    graph.assign(n * m, vector<uint32_t>());
    used.assign(n * m, 0);
    newused.assign(n * m, 0);
    pr.assign(m * n, -1);
    currentIteration = 2, ans = 0;

    for (uint32_t i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        for (uint32_t j = 0; j < m; ++j) {
            if(s[j] == '*') {
                ++count_pl;
                sp[i][j] = 1;
            } else
                sp[i][j] = 0;
        }
    }

    for (uint32_t i = 0; i < n - 1; ++i) {
        for (uint32_t j = 0; j < m - 1; ++j) {
            if(sp[i][j] && sp[i+1][j]) {
                graph[code(i, j)].push_back(code(i + 1, j));
                graph[code(i + 1, j)].push_back(code(i, j));
                maybe_add(code(i, j), code(i + 1, j));
            } if(sp[i][j] && sp[i][j + 1]) {
                graph[code(i, j)].push_back(code(i, j + 1));
                graph[code(i, j + 1)].push_back(code(i, j));
                maybe_add(code(i, j + 1), code(i, j));
            }
        }
    }
    for (uint32_t i = 0; i < n - 1; ++i) {
        uint32_t j = m - 1;
        if(sp[i][j] && sp[i + 1][j]) {
            graph[code(i, j)].push_back(code(i + 1, j));
            graph[code(i + 1, j)].push_back(code(i, j));
            maybe_add(code(i + 1, j), code(i, j));
        }
    }

    for (uint32_t j = 0; j < m - 1; ++j) {
        uint32_t i = n - 1;
        if(sp[i][j] && sp[i][j + 1]) {
            graph[code(i, j)].push_back(code(i, j + 1));
            graph[code(i, j + 1)].push_back(code(i, j));
            maybe_add(code(i, j + 1), code(i, j));
        }
    }

    if(a < b * 2) {
        findMaximalMatching();
        cout << ans * a + (count_pl - ans * 2) * b;
    }else {
        cout << count_pl * b;
    }
    return 0;
}