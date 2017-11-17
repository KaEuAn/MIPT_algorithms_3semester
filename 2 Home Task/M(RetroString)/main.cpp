#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>


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
    u32 zfunc[n + 2];
    makeZFunction(sin, zfunc);
    u32 answer[n + 2];
    u32 toless[n];
    for (int i = 0; i < n; ++i) {
        answer[i] = 0;
        toless[i] = 0;
    }
    u32 toadd = 0;
    for (int i = 1; i < n; ++i) {
        ++toadd;
        ++toless[i + zfunc[i]];
        toadd -= toless[i];
        answer[i] += toadd;
    }
    u32 ma = answer[0];
    u32 index = 0;
    for (int i = 1; i < n; ++i) {
        if (answer[i] > ma) {
            index = i;
            ma = answer[i];
        }
    }
    for (int i = 0; i < index + 1; ++i) {
        printf("%c", sin[i]);
    }
    return 0;
}