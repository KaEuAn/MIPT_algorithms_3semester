#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::map;
using u32 = uint64_t;
using u8 = uint16_t;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::make_pair;




int main() {
    int n = 1, m = 1;
    while(!(n == 0 && m == 0)) {
        cin >> n >> m;
        int array[n];
        int lastLeft[n];
        int lastRight[n];
        int partsum[n];
        for (int i = 0; i < n; ++i) {
            lastLeft[i] = 0;
            lastRight[i] = n;
            cin >> array[i];
        }
    }
    return 0;
}