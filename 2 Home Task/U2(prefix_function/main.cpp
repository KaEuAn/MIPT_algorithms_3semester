#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t ;

void makePrefixFunction(string sin, u32* pfunc) {
    u32 n = sin.length();
    u32* prfunc = pfunc;
    --prfunc;
    prfunc[1] = 0;
    for (int i = 2; i < n + 1; ++i) {
        u32 j;
        for(j = prfunc[i - 1]; j > 0 && sin[j] != sin[i - 1]; j = prfunc[j]) {}
        if (sin[j] == sin[i - 1])
            prfunc[i] = j + 1;
        else
            prfunc[i] = j;
    }
}

int main() {
    string sin;
    cin >> sin;
    u32 n = sin.length();
    u32 prfunc[n + 1];
    makePrefixFunction(sin, prfunc);
    for (int i = 0; i < n; ++i) {
        cout << prfunc[i] << ' ';
    }
    return 0;
}