#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>
using ld = long double;

const ld EPS = 1e-10;

bool isZero(ld x) {
    return -EPS <= x && x <= EPS;
}

bool areEqual(ld x, ld y) {
    return isZero(x - y);
}

bool isLessThan(ld x, ld y) {
    return x < y - EPS;
}

bool isGreaterThan(ld x, ld y) {
    return x >  y + EPS;
}


struct Point {

    ld x;
    ld y;

    Point() : x(0), y(0) {}
    Point(ld first, ld second) : x(first), y(second) {}

    ld length() const {
        return std::sqrt(x * x + y * y);
    }
    ld operator%(const Point& second) const {
        return x * second.y - y * second.x;
    }
    ld operator*(const Point& second) const {
        return x * second.x + y * second.y;
    }
    ld len2() const {
        return x*x + y*y;
    }
    ld len() const {
        return std::sqrt(len2());
    }


    bool operator ==(const Point& a) const {
        return (isZero(x - a.x) && isZero(y - a.y));
    }

    bool operator != (const Point& a) {
        return ! (*this == a);
    }
    Point operator - (const Point& a) const {
        Point c(*this);
        c.x -= a.x;
        c.y -= a.y;
        return c;
    }
    Point& operator += (const Point& a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    Point operator + (const Point& a) const {
        Point c(*this);
        return c += a;
    }
    Point operator / (double a) const {
        Point c(*this);
        c.x /= a;
        c.y /= a;
        return c;
    }
    Point operator * (double a) const {
        Point c(*this);
        c.x *= a;
        c.y *= a;
        return c;
    }
    //friend bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d);
};

using std::cin;
using std::cout;
using std::vector;

bool xCompare(Point a, Point b) {
    if (isZero(a.x - b.x))
        return a.y < b.y;
    return a.x < b.x;
}


bool smartCompare(Point a, Point b) {
    if (isZero(a % b))
        return a.x < b.x;
    return (a % b > 0);
}

bool smart1Compare(Point a, Point b, Point c) {
    ld ans = (a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y));
    return ans < 0 || isZero(ans);
}


int main() {
    cout << std::setprecision(16);
    uint32_t n, m, k;
    cin >> n >> m >> k;
    vector<Point> xy;
    xy.reserve(n);
    int cmpId = 0;
    for(uint32_t i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        xy.push_back(Point(x, y));
        if(xCompare(xy[i], xy[cmpId]))
            cmpId = i;
    }
    std::rotate(xy.begin(), xy.begin() + cmpId, xy.end());;
    Point xy0 = xy[0];
    xy.erase (xy.begin());
    --n;

    std::vector<Point> angle(n);
    for (int i = 0; i < n; ++i) {
        angle[i] = xy[i] - xy0;
        if (angle[i].x == 0)
            angle[i].y = angle[i].y < 0 ? -1 : 1;
    }
    int ans = 0;
    for (int i = 0; i < m; ++i) {
        ld xx, yy;
        cin >> xx >> yy;
        Point mm(xx, yy);
        if(mm.x >= xy0.x) {
            if(mm == xy0)
                ++ans;
            else {
                Point nowAngle = mm - xy0;
                if (nowAngle.x == 0)
                    nowAngle.y = nowAngle.y < 0 ? -1 : 1;
                vector<Point>::iterator it = std::upper_bound(angle.begin(), angle.end(), nowAngle, smartCompare);
                if(it == angle.end() && angle[n - 1] == nowAngle)
                    --it;
                int sdfa = it - angle.begin();
                if(it != angle.end() && it != angle.begin() && smart1Compare(xy[sdfa], xy[sdfa - 1], mm))
                    ++ans;
            }
        }

    }
    if(ans >= k)
        cout << "YES";
    else
        cout << "NO";
    return 0;
}
