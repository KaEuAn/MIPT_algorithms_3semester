#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t;
using u64 = uint64_t;
const u32 INF = 9999999;

int main() {
    u32 n;
    cin >> n;
    u32 p[n], z[n];
    for (int i = 0; i < n; ++i) {
        p[i] = 0;
    }
    cin >> z[0];
    for (int i = 1; i < n; ++i) {
        cin >> z[i];

        for (int j = i + z[i] - 1; j >= i && !(p[j]); --j) {
            p[j] = j + 1 - i;
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << p[i] << ' ';
    }
    return 0;
}