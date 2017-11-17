#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
const long long INF =  9999999999999;
const long long INFF = 99999999999999999;
using std::string;
using std::cout;
using std::cin;

using u32 = uint32_t;

vector<vector<long long>> graph;
vector<long long> firstPotential, secondPotential, minj, lineForColumn, columnMin;
vector<uint8_t > used;
long long n;

long long count = 0;
vector<vector<u32>> ans;

long long a[51][51];

void makeOneIteration(long long iterationNumber) {
    firstPotential.assign(n, 0);
    secondPotential.assign(n, 0);
    lineForColumn.assign(n, 0);
    columnMin.assign(n, 0);
    for (u32 i = 1; i < n; ++i) {
        used.assign(n, 0);
        minj.assign(n, INFF);
        lineForColumn[0] = i;
        u32 currentColumn = 0;
        do {
            used[currentColumn] = 1;
            long long curChosenLine = lineForColumn[currentColumn], delta = INFF * 50, nextColumn;
            //updating potentials
            for (long long j = 1; j < n; ++j) {
                if (used[j] == 0) {
                    long long currentPot = a[curChosenLine][j] - firstPotential[curChosenLine] - secondPotential[j];
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
    count -= secondPotential[0];
    for (int i = 1; i < n; ++i) {
        ans[iterationNumber][lineForColumn[i]] = i;
    }
}

int main() {
    long long k;
    cin >> n >> k;
    ++n;
    ans.assign(k, vector<u32>(n, 0));
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < k; ++i) {
        makeOneIteration(i);
        for (int j = 1; j < n; ++j) {
            a[j][ans[i][j]] += INF;
        }
    }
    cout << count << '\n';
    for (int i = 0; i < k; ++i) {
        for (int j = 1; j < n; ++j) {
            cout << ans[i][j] << ' ';
        }
        cout << '\n';
    }
    return 0;
}