#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>
using ld = long double;
using std::cin;
using std::cout;
using std::vector;


const ld EPS = 1e-6;

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
    ld returny(ld bb) const {
        return (-c - a * bb)/b;
    }
};

struct segment {
    Point a;
    Point b;
};

bool isInTheMiddle(Point first, Point second, Point third) {
    Point zer = second - first;
    Point ter = third - first;
    if(areCollinear(zer, ter) && isLessThan(zer * ter, 0) ){
        return true;
    } return (first == second) || (first == third);
}

void intersectAndAdd(Point s1p1, Point s1p2, Point s2p1, Point s2p2, vector<Point>& ans) {
    Line first(s1p1, s1p2);
    Line second(s2p1, s2p2);
    if(first.areIntersects(second)) {
        Point addp = (first.intersection(second));
        if (isInTheMiddle(addp, s1p1, s1p2) && isInTheMiddle(addp, s2p1, s2p2))
            ans.push_back(addp);

    }return;
}

bool isSimilar(ld a, ld b) {
    return std::abs(a - b) < EPS;
}

struct item{
    Point p;
    int id;
    item(ld a, ld b, int c) : p(a, b), id(c) {}
    item() = default;
};

ld makeProektion(segment a, ld b) {
    Line dsadsa(a.a, a.b);
    return dsadsa.returny(b);
}
vector<item> c;
bool ycmp(int i, int j) {
    item a = c[i];
    item b = c[j];
    return isLessThan(a.p.x, b.p.x) || isZero(a.p.x - b.p.x) && isLessThan(a.p.y, b.p.y);
}

int main() {
    vector<segment> seg(6);
    for (int i = 0; i < 2; ++i) {
        ld a1, a2, a3, a4, a5, a6;
        cin >> a1 >> a2 >> a3 >> a4 >> a5 >> a6;
        Point fir(a1, a2);
        Point sec(a3, a4);
        Point thi(a5, a6);
        seg[i * 3] = {fir, sec};
        seg[i * 3 + 1] = {sec, thi};
        seg[i * 3 + 2] = {thi, fir};
    }
    for (int i = 0; i < 6; ++i) {
        if(seg[i].a.x > seg[i].b.x) {
            std::swap(seg[i].a, seg[i].b);
        }
    }
    vector<Point> inter;
    inter.reserve(12);
    for (int i = 0; i < 6; ++i) {
        for (int j = i + 1; j < 6; ++j) {
            intersectAndAdd(seg[i].a, seg[i].b, seg[j].a, seg[j].b, inter);
        }
    }

    vector<ld> xs (inter.size());
    for (int i = 0; i < inter.size(); ++i)
        xs[i] = inter[i].x;
    sort (xs.begin(), xs.end());
    xs.erase (std::unique (xs.begin(), xs.end(), &isSimilar), xs.end());
    ld ans = 0;
    vector<bool> used(6);
    c.resize(18);
    vector<int> cIndex(18);
    for (int i = 0; i < xs.size() - 1; ++i) {
        ld first = xs[i];
        ld second = xs[i+1] ;
        int ind = 0;
        for (int j = 0; j < seg.size(); ++j)
            if (seg[j].a.x != seg[j].b.x)
                if ( (isLessThan(seg[j].a.x, first) || isZero(seg[j].a.x - first)) &&
                        (isGreaterThan(seg[j].b.x, second) || isZero(seg[j].b.x - second))) {
                    //проекция на ось y;
                    item it = {makeProektion(seg[j], first), makeProektion(seg[j], second), j / 3 };
                    cIndex[ind] = ind;
                    c[ind] = it;
                    ++ind;
                }
        sort (cIndex.begin(), cIndex.begin()+ind, &ycmp);
        ld ans2 = 0;
        for (int j = 0; j < ind; ) {
            item lower = c[cIndex[j]];
            ++j;
            used[lower.id] = true;
            int cnt = 1;
            while (cnt && j<ind) {
                bool current = !used[c[cIndex[j]].id];
                used[c[cIndex[j]].id] = current;
                j++;
                if (current)
                    ++cnt;
                else
                    --cnt;
            }
            item upper = c[cIndex[j-1]];
            ans2 += upper.p.x - lower.p.x + upper.p.y - lower.p.y;
        }
        ans += ans2 * (second - first) / 2;
    }
    cout << std::setprecision(16);
    double anss = ans;
    printf("%.16f", anss);

    return 0;
}