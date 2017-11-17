#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>
using ld = long double;

const ld EPS = 1e-8;
const ld INF = 99999999999999;

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
    Point& operator=(const Point& other){
        if(this == &other)
            return *this;
        x = other.x;
        y = other.y;
        return *this;
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

    bool operator < (Point b) {
        if (isZero(x - b.x))
            return y < b.y;
        return x < b.x;
    }
    //friend bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d);
};
using std::cout;
using std::cin;
using std::vector;

uint32_t n;
Point* xxxxxx;
Point* t;
ld mindistance = INF;

bool ycmp(Point a, Point b) {
    return a.y < b.y;
}
bool xcomp (Point a, Point b) {
    if (isZero(a.x - b.x))
        return a.y < b.y;
    return a.x < b.x;
}


void update(Point first, Point second){
    mindistance = std::min(mindistance, (first - second).length());
}

void func(int first, int second) {
    ld col = second - first;
    if(col <= 4) {
        std::sort(xxxxxx + first, xxxxxx + second, &ycmp);
        for (int i = first; i < second; ++i) {
            for (int j = i + 1; j < second; ++j) {
                update(xxxxxx[i], xxxxxx[j]);
            }
        }
        return;
    }
    int middle = (first + second) / 2;
    func(first, middle);
    func(middle, second);
    ld middle_x = xxxxxx[middle].x;
    std::merge(xxxxxx + first, xxxxxx + middle , xxxxxx + middle, xxxxxx + second, t, &ycmp);
    std::copy(t, t + second - first, xxxxxx + first);
    int tssss = 0;
    //спукаемся вниз, пока можем. Вверх - пропускаем пары, не получилось.
    //буфер - чтобы уменьшить количество пробеганий
    for (int i = first; i < second; ++i) {
        if(std::abs(xxxxxx[i].x - middle_x) < mindistance * 2){
            for (int j = tssss - 1; j >= 0 && std::abs(t[j].y - xxxxxx[i].y) <= mindistance * 2; --j) {
                update(xxxxxx[i], t[j]);
            }
            t[tssss] = xxxxxx[i];
            ++tssss;
        }
    }
    return;
}

int main() {
    cin >> n;
    cout << std::setprecision(16);
    t = new Point[n + 5];
    xxxxxx = new Point[n + 5];
    for (int i = 0; i < n; ++i) {
        ld a, b;
        cin >> a >> b;
        xxxxxx[i] = Point(a,b);
    }
    std::sort(xxxxxx, xxxxxx + n);
    func(0, n);
    cout << mindistance;
    delete[] t;
    delete[] xxxxxx;
    return 0;
}