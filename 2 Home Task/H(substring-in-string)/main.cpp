#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t ;
void makeZFunction(string sin, u32* zfunc){
    u32 n = sin.length();
    u32 l = 0, r = 0;
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


int main() {
    string a, b;
    cin >> a >> b;
    string sin(b + '#' + a);
    u32 zfunc[sin.length() + 2];
    makeZFunction(sin, zfunc);
    u32 ans = 0;
    vector<u32> answers;
    for (int i = b.length() + 1; i < sin.length(); ++i) {
        if (zfunc[i] == b.length()) {
            ++ans;
            answers.push_back(i - b.length());
        }
    }
    cout << ans << '\n';
    for (int i = 0; i < ans; ++i) {
        cout << answers[i] << ' ';
    }
    return 0;
}