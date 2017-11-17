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
        cin >> p[i];
        z[i] = 0;
        z[i - p[i] + 1] = std::max(p[i], z[i - p[i] + 1]);
    }
    z[0] = n;
    for (int i = 1, x = i; i < n; i = x + 1) {
        x = i;
        if (z[i] > 0) {
            for (int j = 0; j < z[i]; ++j) {
                if (z[j] < z[i + j])
                    //than will be similar because of p-function
                    break;
                z[i + j] = std::min(z[j], z[i] - j);
                x = i + j;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << z[i] << ' ';
    }
    return 0;
}