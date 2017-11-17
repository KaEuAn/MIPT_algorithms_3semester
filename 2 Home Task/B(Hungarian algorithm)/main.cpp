#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
const int INF = 99999999;
using std::string;
using std::cout;
using std::cin;
using u32 = uint32_t;

vector<vector<uint32_t>> graph;
vector<uint32_t> firstPotential, secondPotential, minj, lineForColumn, columnMin;
vector<uint8_t > used;
u32 n;

u32 getDelta() {
    u32 min = 0;
    for (int i = 0; i < minj.size(); ++i) {
        min = min < minj[i] ? min : minj[i];
    }
    return min;
}



int main() {
    cin >> n;
    ++n;
    u32 a[n][n];
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            cin >> a[i][j];
        }
    }
    firstPotential.assign(n, 0);
    secondPotential.assign(n, 0);
    lineForColumn.assign(n, 0);
    columnMin.assign(n, 0);
    for (u32 i = 1; i < n; ++i) {
        used.assign(n, 0);
        minj.assign(n, INF);
        lineForColumn[0] = i;
        u32 currentColumn = 0;
        do {
            used[currentColumn] = 1;
            u32 curChosenLine = lineForColumn[currentColumn], delta = INF, nextColumn;
            //updating potentials
            for (u32 j = 1; j < n; ++j) {
                if (used[j] == 0) {
                    u32 currentPot = a[curChosenLine][j] - firstPotential[curChosenLine] - secondPotential[j];
                    if (currentPot < minj[j]) {
                        minj[j] = currentPot;
                        columnMin[j] = currentColumn;
                    }
                    if (minj[j] < delta) {
                        delta = minj[j];
                        nextColumn = j;
                    }
                }
            }
            for (int j = 0; j < n; ++j) {
                if (used[j]) {
                    firstPotential[lineForColumn[j]] += delta;
                    secondPotential[j] -= delta;
                } else {
                    minj[j] -= delta;
                }
            }
            currentColumn = nextColumn;
        } while(lineForColumn[currentColumn] != 0);
        do {
            u32 previousColumn = columnMin[currentColumn];
            lineForColumn[currentColumn] = lineForColumn[previousColumn];
            currentColumn = previousColumn;
        } while (currentColumn);
    }
    cout << secondPotential[0] * -1 << '\n';
    vector<u32> answer(n);
    for (int i = 1; i < n; ++i) {
        cout << lineForColumn[i] << ' ' << i << '\n';
    }
    return 0;
}