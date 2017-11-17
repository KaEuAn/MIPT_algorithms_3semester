#include <algorithm>
#include <cstdio>
#include <iostream>
#include <math.h>

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

private:
    ld x;
    ld y;
public:
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
    friend bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d);
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
    Line (ld k, ld b) :  a(-k), b(1), c(-b) {};

    Line (Point p, ld k) :  a(-k), b(1), c(k * p.x - p.y) {};

    Line () : a(1), b(1), c(0) {};
    friend class Point;
    Line(const Point& first, const Point& second) :
            a(first.y - second.y),
            b(second.x - first.x),
            c(-a * first.x - b * first.y) {}
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
    bool isOnTheLine(Point p) const {
        return isZero(a * p.x + b * p.y + c);
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

bool isInTheMiddle(Point first, Point second, Point third) {
    Point zer = second - first;
    Point ter = third - first;
    if(areCollinear(zer, ter) && isLessThan(zer * ter, 0) ){
        return true;
    } return (first == second) || (first == third);
}

bool isOnTheRay(Point a, Point start, Point t) {
    return isInTheMiddle(a, start, t) || isInTheMiddle(t, start, a);
}

int main() {
    using std::cin;
    using std::cout;
    long double a, b, c, d;
    cin >> a >> b >> c >> d;
    cout << std::setprecision(16);
    Point start1(a,b);
    Point ver1(c,d);
    Line line1(start1, ver1);
    cin >> a >> b >> c >> d;
    Point start2(a,b);
    Point ver2(c,d);
    Line line2(start2, ver2);
    Line linePerpendicular2 = line2.makePerpendicularLine(start1);
    Line linePerpendicular1 = line1.makePerpendicularLine(start2);
    ld ans = (start1 - start2).length();
    if ((isOnTheRay(start1, start2, ver2))
        || (isOnTheRay(start2, start1, ver1) ))
        ans = 0;
    Point intersect0 = line1.intersection(line2);
    if(isOnTheRay(intersect0, start1, ver1) && isOnTheRay(intersect0, start2, ver2))
        ans = 0;
    Point intersect2 = line2.intersection(linePerpendicular2);
    Point intersect1 = line1.intersection(linePerpendicular1);
    if (isOnTheRay(intersect1, start1, ver1))
        ans = std::min(ans, (intersect1 - start2).length());
    if (isOnTheRay(intersect2, start2, ver2))
        ans = std::min(ans, (intersect2-start1).length());
    cout << ans;

    return 0;
}