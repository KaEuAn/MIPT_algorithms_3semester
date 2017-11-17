#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <cstdio>


using std::vector;
using std::cout;
using std::cin;
using std::string;
using u32 = uint32_t;
using u64 = uint64_t;


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

bool isOneInsideOtherWithCycleShift(const string &a, const string &b) {
    string nstr = a + "\n" + b + b;
    u32 zfunc[nstr.length()];
    makeZFunction(nstr, zfunc);
    for (int i = a.length(); i < nstr.length(); ++i) {
        if (zfunc[i] == a.length())
            return true;
    }
    return false;
}


u32 lengthOfPeriod(const string& a) {
    u32 n = a.length();
    u32 zfunc[n + 2];
    makeZFunction(a, zfunc);
    u32 answer = 0;
    for (u32 i = 1; i < n; ++i) {
        if (i + zfunc[i] == n) {
            answer = i;
            break;
        }
    }
    if (answer == 0)
        return n;
    return answer;
}

string makePeriod(const string& a, u32 len) {
    string answer(a, 0, len);
    return answer;
}

u32 maxIntersectWhenConcatenate(const string& a, const string& b) {
    string nstr = b + "\n" + a;
    u32 pfunc[nstr.length()];
    makePrefixFunction(nstr, pfunc);
    return pfunc[nstr.length() - 1];
}

int main() {
    string first, second;
    cin >> first >> second;
    if (first.length() > second.length())
        std::swap(first, second);
    string ff = makePeriod(first, lengthOfPeriod(first));
    string ss = makePeriod(second, lengthOfPeriod(second));
    if (isOneInsideOtherWithCycleShift(first, ss)) {
        cout << ss;
    } else if(isOneInsideOtherWithCycleShift(second, ff)) {
        cout << ff;
    } else if(ff.length() == ss.length() && isOneInsideOtherWithCycleShift(ff, ss)) {
        cout << ss;
    }
    else{
        u32 n1 = maxIntersectWhenConcatenate(first, second);
        u32 n2 = maxIntersectWhenConcatenate(second, first);
        if (n1 < n2) {
            cout << string(second, 0, second.length() - n2) + first;
        }
        else
            cout << string(first, 0, first.length() - n1) + second;
    }
     return 0;
}