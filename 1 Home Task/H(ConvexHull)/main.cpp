#include <algorithm>
#include <cstdio>
#include <iostream>
#include <math.h>

#include <iomanip>

using ld = long double;

const ld EPS = 1e-15;

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
    Point() : x(0), y(0) {};
    Point(const Point& a) : x(a.x), y(a.y) {}
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
    friend class Line;
    //friend bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d);
};

bool areCollinear(const Point& first, const Point& second) {
    return isZero(first % second);
}

bool arePerpendicular(const Point& first, const Point& second) {
    return isZero(first * second);
}

struct Line {
private:
    ld a;
    ld b;
    ld c;
    // ax + by + c = 0
public:
    Line (Point one, Point two) : a(one.y - two.y), b(two.x - one.x), c(one.x * two.y - two.x * one.y) {};

    Line (ld k, ld b) :  a(-k), b(1), c(-b) {};

    Line (Point p, double k) :  a(-k), b(1), c(k * p.x - p.y) {};

    Line () : a(1), b(1), c(0) {};
    friend class Point;
    Line(const Point& first, const Point& second) :
            a(first.y - second.y),
            b(second.x - first.x),
            c(-a * first.x - b * second.y) {}
    bool areIntersects(const Line& second) const {
        return !areCollinear(Point{b, -a}, Point{second.b, -second.a});
    }
    Point intersection(const Line& line) const {
        Point answer;
        Point normal(a, b);
        Point secondNormal(line.a, line.b);
        ld z = secondNormal % normal;
        if (!isZero(z)) {
            answer.y = (line.c * a - c * line.a) / (z);
            if (a != 0)
                answer.x = (-c - b * answer.y) / a;
            else
                answer.x = (-line.c - line.b * answer.y) / line.a;
        } else {
            answer.x = a == 0 ? -line.c / line.a : -c / a;
            answer.y = b == 0 ? -line.c / line.b : -c / b;
        }
        return answer;
    }


    void normalize() {
        a /= std::sqrt(a*a + b*b);
        b /= std::sqrt(a*a + b*b);
    }
    Line makePerpendicularLine(Point newPoint) {
        Line answer;
        answer.a = - b;
        answer.b = a;
        answer.c = - answer.a * newPoint.x - answer.b * newPoint.y;
        return answer;
    }
};

bool intersectsld(ld a, ld b, ld c, ld d) {
    if (isGreaterThan(a, b))
        std::swap(a, b);
    if (isGreaterThan(c, d))
        std::swap(c, d);
    return !(isLessThan(b, c) || isLessThan(d, a));
}
int siqn(ld x) {
    if (isZero(x))
        return 0;
    return isLessThan(x, 0) ? -1 : 1;
}

/*bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d) {
    return intersectsld(a.x, b.x, c.x, d.x) && intersectsld(a.y, b.y, c.y, d.y) &&
            (sign((b-a)%(c-a)) == 1) && (1);
}*/


using std::cin;
using std::cout;
using std::vector;

bool xCompare(Point a, Point b) {
    if (isZero(a.x - b.x))
        return a.y < b.y;
    return a.x < b.x;
}
Point cmpPoint;
//isLess
bool smartCompare(Point b, Point c) {
    ld ans = (b - cmpPoint) % (c - cmpPoint);
    return (isGreaterThan(ans, 0));
}

bool smartCompare1(Point cmpPoint, Point b, Point c) {
    ld ans = (b - cmpPoint) % (c - cmpPoint);
    return (isGreaterThan(ans, 0));
}
bool smart___Compare(Point cmpPoint, Point b, Point c) {
    ld ans = (b - cmpPoint) % (c - cmpPoint);
    return (isLessThan(ans, 0));
}


vector<Point> makeConvexHull_first(vector<Point>& xy, uint32_t n) {
    vector<Point> convexHull;
    convexHull.reserve(n);
    std::sort(xy.begin(), xy.end(), xCompare);
    cmpPoint = xy[0];
    std::sort(++xy.begin(), xy.end(), smartCompare);
    for (int i = 1; i < n; ++i) {
        while (convexHull.size() >= 2 && !smartCompare1 (convexHull[convexHull.size() - 2],
                                                        convexHull[convexHull.size() - 1], xy[i]))
            convexHull.pop_back();
        convexHull.push_back(convexHull[i]);
    }
    return convexHull;
}
void makeConvexHull_second(vector<Point>& xy) {
    if (xy.size() == 1) return;
    vector<Point> convexHull;
    std::sort(xy.begin(), xy.end(), xCompare);
    Point cmpPoint1 = xy[0];
    Point cmpPoint2 = xy.back();
    vector<Point> verx, niz;
    verx.push_back(cmpPoint1);
    niz.push_back(cmpPoint1);
    for (int i = 1; i < xy.size(); ++i) {
        if (i==xy.size() - 1 || smartCompare1(cmpPoint1, xy[i], cmpPoint2)) {
            while (verx.size() >= 2 && !smartCompare1 (verx[verx.size() - 2], verx[verx.size() - 1], xy[i]))
                verx.pop_back();
            verx.push_back (xy[i]);
        }
        if (i==xy.size() - 1 ||  smart___Compare(cmpPoint1, xy[i], cmpPoint2)) {
            while (niz.size()  >= 2 && !smart___Compare(niz[niz.size()  - 2], niz[niz.size()  - 1], xy[i]))
                niz.pop_back();
            niz.push_back(xy[i]);
        }
    }
    xy.clear();
    for (int i = 0; i < verx.size(); ++i) {
        xy.push_back(verx[i]);
    }
    for (int j = niz.size() - 1; j > 0 ; --j) {
        xy.push_back(niz[j]);
    }
}

int main() {
    cout << std::setprecision(16);
    uint32_t n, l;
    cin >> n >> l;
    vector<Point> xy;
    xy.reserve(n);
    for (int i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        xy.push_back(Point(x, y));
    }
    xy = makeConvexHull_first(xy, n);
    ld ans = 2 * l * M_PI;
    for (int j = 1; j < xy.size(); ++j) {
        ans += (xy[j] - xy[j - 1]).length();
    }
    ans += (xy[0] - xy[xy.size() - 1]).length();
    cout << ans;
    return 0;
}