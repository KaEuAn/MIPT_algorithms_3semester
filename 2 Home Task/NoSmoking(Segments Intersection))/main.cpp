#include <algorithm>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <set>

using ld = long double;
using u32 = uint32_t;
using std::cin;
using std::cout;
using std::vector;
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
struct Segment;

struct Point {

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
    bool operator <(const Point& a) const {
        if (x == a.x) {
            return y < a.y;
        }
        return x < a.x;
    }
    bool operator >(const Point& a) const {
        return a < *this;
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


struct Segment {
    Point start;
    Point finish;
    u32 number;
    Segment(const Point& a, const Point& b, u32 n) : start(a), finish(b), number(n) {
        if (start > finish)
            std::swap(start, finish);
    }
    Segment(ld a, ld b, ld c, ld d, u32 n) : start(a, b), finish(c, d), number(n) {
        if (start > finish)
            std::swap(start, finish);
    }

    bool operator ==(const Segment& a) const {
        return start == a.start && finish == a.finish;
    }
    bool operator < (const Segment& b) const {
        if (start.x == b.start.x) {
            if (start.y == b.start.y) {
                Point first = finish - start;
                Point second = b.finish - b.start;
                return first % second > 0;
            }
            return start.y < b.start.y;
        }
        const Segment &second = (start.x < b.start.x ? b : *this);
        const Segment &first = (start.x < b.start.x ? *this : b);
        ld y1 = (second.start.x - first.start.x) * (first.finish.y - first.start.y)
                + first.start.y * (first.finish.x - first.start.x);
        bool is = y1 < second.start.y * (first.finish.x - first.start.x);
        if (first.start.x == start.x)
            return is;
        return !is;
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

bool intersectsld(ld a, ld b, ld c) {
    if (isGreaterThan(a, b))
        std::swap(a, b);
    return !(isLessThan(b, c) || isLessThan(c, a));
}
int sign(ld x) {
    if (isZero(x))
        return 0;
    return isLessThan(x, 0) ? -1 : 1;
}

bool areSegmentrsIntersects(const Point& a, const Point& b, const Point& c, const Point& d) {
    Line first(a, b);
    Line second(c, d);
    if (!first.areIntersects(second))
        return false;
    Point inter = first.intersection(second);
    if (intersectsld(a.x, b.x, inter.x) && (c.x, d.x, inter.x));
        return true;
}
bool areSegmentrsIntersects(const Segment& a, const Segment& b) {
    return areSegmentrsIntersects(a.start, a.finish, b.start, b.finish);
}

bool isInTheMiddle(Point first, Point second, Point third) {
    Point zer = second - first;
    Point ter = third - first;
    if(areCollinear(zer, ter) && isLessThan(zer * ter, 0) ){
        return true;
    } return (first == second) || (first == third);
}

/*bool firstXCompare(const Segment& a, const Segment& b) {
    if (isLessThan(a.a.x, b.a.x))
        return true;
    return isLessThan(a.b.x, b.b.x);
}*/



enum Type{
    in,
    query,
    out
};


struct Event{
    Point point;
    Type action;
    Segment* segment;
    u32 number;

    Event(Point a, Type b, Segment* c, u32 n = 0) : point(a), action(b), segment(c), number(n) {}
    bool operator <(const Event& a) const {
        if (point == a.point) {
            return action < a.action;
        }
        if (point.x == a.point.x)
            return point.y < a.point.y;
        return point.x < a.point.x;
    }
};



bool isHigher(Point a, Segment b) {
    if (b.start.x == b.finish.x)
        return a.y > b.finish.y;
    return (a.x - b.start.x) * (b.finish.y - b.start.y) +
           b.start.y * (b.finish.x - b.start.x) < a.y * (b.finish.x - b.start.x);
}
bool contains(Point a, Segment b) {
    if (b.start.x == b.finish.x)
        return (a.x == b.start.x) && (a.y <= b.finish.y && b.start.y <= a.y);
    ld x = (a.x - b.start.x) * (b.finish.y - b.start.y);
    return areEqual(x + b.start.y * (b.finish.x - b.start.x), a.y * (b.finish.x - b.start.x));
}

class solveTask{
    u32 n;
    vector<Segment> Polygon;
    std::vector<Event> events;
    bool answer;
    u32 aranswer[2];

    void preparation() {
        std::sort(events.begin(), events.end());
    }

    void addSegment(Segment* a) {
        events.push_back(Event(a->start, in, a));
        events.push_back(Event(a->finish, out, a));
    }
    void printAnswer() const {
        if (! answer)
            cout << "NO";
        else
            cout << "YES" << '\n' << aranswer[0] << ' ' << aranswer[1];
    }

    void makeAnswer() {
        std::set<Segment> tree;
        for(auto it: events) {
            if (it.action == in) {
                tree.insert(*(it.segment));

            } else{
                std::set<Segment>::iterator fi = tree.find(*(it.segment));
                auto li = fi;
                auto hi = fi;
                ++hi;
                --li;
                if (fi != tree.begin() &&
                        (! isHigher(fi->finish, *li) || contains(li->start, *fi))) {
                    answer = true;
                    aranswer[0] = fi->number;
                    aranswer[1] = li->number;
                    break;
                }
                if (hi != tree.end() &&
                        (isHigher(fi->finish, *hi) || contains(fi->finish, *hi) || contains(hi->start, *fi))) {
                    answer = true;
                    aranswer[0] = fi->number;
                    aranswer[1] = hi->number;
                    break;
                }
                tree.erase(fi);
            }
        }
    }

public:
    solveTask() : answer(false) {}

    void run(const vector<Segment>& segments) {
        n = segments.size();
        Polygon = segments;
        events.reserve(n * 2);
        for (u32 i = 0; i < n; ++i) {
            if(Polygon[i].start == Polygon[i].finish)
                continue;
            addSegment(&Polygon[i]);
        }
        preparation();
        makeAnswer();
        printAnswer();
    }


};


int main() {
    u32 n;
    vector<Segment> segments;
    cin >> n;
    for (u32 i = 0; i < n; ++i) {
        ld a, b, c, d;
        cin >> a >> b >> c >> d;
        segments.push_back(Segment(a, b, c, d, i + 1));
    }
    solveTask task;
    task.run(segments);
    return 0;
}