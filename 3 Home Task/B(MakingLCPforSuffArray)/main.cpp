#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using u32 = uint32_t;

vector<u32> makeLCP (const vector<u32>& suff, const string& s) {
    u32 n = suff.size();
    vector<u32> posInSuff(n);
    vector<u32> LCP(n);
    for (int i = 0; i < n; ++i) {
        posInSuff[suff[i]] = i;
    }
    u32 index = 0;
    for (int i = 0; i < n; ++i) {
        if (index != 0)
            --index;
        if (posInSuff[i] == n - 1) {
            index = 0;
        } else {
            for (u32 j = suff[posInSuff[i] + 1];
                 j + index < n && i + index < n && s[i + index] == s[j + index]; ++index) {}
            LCP[posInSuff[i]] = index;
        }
    }
    return LCP;
}

int main() {
    u32 n;
    cin >> n;
    vector<u32> suff(n + 1);
    string s;
    cin >> s;
    s.push_back(0);
    for (int i = 0; i < n; ++i) {
        cin >> suff[i + 1];
        --suff[i + 1];
    }
    suff[0] = n - 1;
    auto lcp = makeLCP(suff, s);
    for (int i = 1; i < n; ++i) {
        cout << lcp[i] << ' ';
    }
    return 0;
}