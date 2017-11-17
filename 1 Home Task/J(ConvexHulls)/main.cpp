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
    uint32_t number;
    uint32_t component;

    Point() : x(0), y(0), number(0), component(0) {}
    Point(ld first, ld second, uint32_t n) : x(first), y(second), number(n), component(0) {}

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

bool xCompare(Point* a, Point* b) {
    if (a->component != b->component)
        return a->component < b->component;
    if (isZero(a->x - b->x))
        return a->y < b->y;
    return a->x < b->x;
}
Point cmpPoint;
//isLess
bool smartCompare(Point* a, Point* b) {
    if (a->component != b->component)
        return a->component < b->component;
    ld ans = (*a - cmpPoint) % (*b - cmpPoint);
    return isLessThan(0, ans);
}

bool numberCompare(Point* a, Point* b) {
    return a->number < b->number;
}

bool smartCompare1(Point cmpPoint, Point b, Point c) {
    ld ans = (b - cmpPoint) % (c - cmpPoint);
    return (! isLessThan(ans, 0));
}
bool smart___Compare(Point cmpPoint, Point b, Point c) {
    ld ans = (b - cmpPoint) % (c - cmpPoint);
    return (! isGreaterThan(ans, 0));
}


vector<Point*> makeConvexHull_second(vector<Point*>& xy, int n, int last) {
    vector<Point*> convexHull;
    if (last == 1) {
        int i = 0;
        while(xy[i]->component != 0) ++i;
        convexHull.push_back(xy[i]);
        return convexHull;
    }
    int i = 0;
    int j = n-1;
    while(xy[i]->component != 0) ++i;
    while(xy[j]->component != 0) --j;
    Point* cmpPoint1 = xy[i];
    Point* cmpPoint2 = xy[j];
    vector<Point*> verx, niz;
    verx.push_back(cmpPoint1);
    niz.push_back(cmpPoint1);
    for (int newi = i + 1; newi <= j; ++newi) {
        if (xy[newi]->component != 0) continue;
        if (newi == j || smartCompare1(*cmpPoint1, *xy[newi], *cmpPoint2)) {
            while (verx.size() >= 2 && !smartCompare1 (*verx[verx.size() - 2], *verx[verx.size() - 1], *xy[newi]))
                verx.pop_back();
            verx.push_back (xy[newi]);
        }
        if (newi == j ||  smart___Compare(*cmpPoint1, *xy[newi], *cmpPoint2)) {
            while (niz.size()  >= 2 && !smart___Compare(*niz[niz.size()  - 2], *niz[niz.size()  - 1], *xy[newi]))
                niz.pop_back();
            niz.push_back(xy[newi]);
        }
    }
    for (i = 0; i < verx.size(); ++i) {
        convexHull.push_back(verx[i]);
    }
    for (j = niz.size() - 2; j > 0 ; --j) {
        convexHull.push_back(niz[j]);
    }
    return convexHull;
}


int main() {
    uint32_t n, l;
    cin >> n;
    vector<Point*> xy;
    xy.reserve(n);
    for(uint32_t i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        xy.push_back(new Point(x, y, i));
    }
    int lastNumber = n;
    std::sort(xy.begin(), xy.end(), xCompare);
    for (uint32_t j = 1; lastNumber > 0 ; ++j) {
        vector<Point*> convexHull = makeConvexHull_second(xy, n, lastNumber);
        for (vector<Point*>::iterator it = convexHull.begin(); it != convexHull.end() ; ++it) {
            (*it)->component = j;
            --lastNumber;
        }
    }
    std::sort(xy.begin(), xy.end(), numberCompare);
    for (auto it = xy.begin(); it != xy.end() ; ++it) {
        std::cout << (*it)->component << '\n';
    }
    for(uint32_t i = 0; i < n; ++i) {
        delete xy.back();
        xy.pop_back();
    }
    return 0;
}
