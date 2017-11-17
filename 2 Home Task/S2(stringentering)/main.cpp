#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t;
using u64 = uint64_t;

const u64 modul2 = 27644437;
const u64 mult2 = 919;
const u64 modul1 = 10000000007;
const u64 mult1 = 449;

void makeZFunction(string sin, u64* zfunc){
    u64 n = sin.length();
    u64 l = 0, r = 0;
    zfunc[0] = n;
    for (u32 i = 1; i < n; ++i) {
        if (r < i) {
            zfunc[i] = 0;
        } else {
            zfunc[i] = std::min(r - i, zfunc[i - l]);
        }
        while (i + zfunc[i] < n && sin[zfunc[i] + i] == sin[zfunc[i]]) {
            ++zfunc[i];
        }
        if (r < i + zfunc[i] - 1) {
            r = i + zfunc[i] - 1;
            l = i;
        }
    }
}

u64 code(u64 a, u64 b) {
    return a + b * modul1;
}

int main() {
    string sin;
    cin >> sin;
    u64 answer = 0;
    std::unordered_set<u64> counted;
    for (int i = 0; i < sin.length(); ++i) {
        string newstr(&sin[0] + i, sin.length() - i);
        u64 z_func[newstr.length()];
        makeZFunction(newstr, z_func);
        u64 maxi = 0;
        for (u64 j = 1; j < newstr.length(); ++j) {
            u64 mnew = std::min(j, z_func[j]);
            if (mnew > maxi)
                maxi = mnew;
        }
        u64 hash1 = 0;
        u64 hash2 = 0;
        for (u64 k = 0; k < maxi; ++k) {
            hash1 = ((hash1 * mult1) + newstr[k] ) % modul1;
            hash2 = ((hash2 * mult2) + newstr[k] ) % modul2;
            counted.insert(code(hash1, hash2));
        }
    }
    cout << counted.size();
    return 0;
}