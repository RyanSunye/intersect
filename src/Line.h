//
// Created by HP on 2020/3/4.
//

#ifndef INTERSECTPROJECT_LINE_H
#define INTERSECTPROJECT_LINE_H

#include <queue>
#include <set>
#include <cmath>
#include <string>
#include <sstream>
#include <map>

#define EPS (1e-10)
#define equals(a, b) (fabs((a) - (b)) < EPS)
#define DEBUG 0

using namespace std;

string DoubleToString(double value, unsigned int precision = 6);

class Node;

class Dot : public pair<double, double> {

public:
    Dot(double a, double b) {
        first = a;
        second = b;
    }

    inline Dot operator+(Dot dot) { return {first + dot.first, second + dot.second}; }

    inline Dot operator-(Dot dot) { return {first - dot.first, second - dot.second}; }

    inline Dot operator*(double t) { return {first * t, second * t}; }

    inline Dot operator/(double t) { return {first / t, second / t}; }

    inline double norm() { return first * first + second * second; }

    inline double abs() { return sqrt(norm()); }

    inline void unify() { *this = *this / abs(); }

    bool operator<(const Dot &p) const {
        return !equals(first, p.first) ? first < p.first : second < p.second;
    }

    bool operator==(const Dot &p) const {
        return equals(first, p.first) && equals(first, p.first);
    }

    inline bool onray(Dot *s, Dot *t) {
        return (first - s->first) * (t->first - s->first) >= 0 && (second - s->second) * (t->second - s->second) >= 0;
    }

    inline bool onsegment(Dot *s, Dot *t) {
        return (first - s->first) * (first - t->first) <= 0 && (second - s->second) * (second - t->second) <= 0;
    }

    string tostring() const {
        return "(" + DoubleToString(first) + "," + DoubleToString(second) + ")";
    }
};

class Diagram {
public:
    Node *node = nullptr;

    virtual ~Diagram() = default;

    virtual string tostring() = 0;

    void intersect(Diagram *diagram);
};

class Line : public Diagram {
public:
    char type;
    double a, b, c;
    Dot *s, *t;

    Line(int x0, int y0, int x1, int y1, char ty = 'L');

    Line(double aa, double ab, double ac);

    bool parallel(Line *l);

    void intersect(Line *l);

    Dot *intersect0(Line *l);

    string tostring() override {
        string ss(1, type);
        return ss + ":" + DoubleToString(a) +"x" + DoubleToString(b)+"y" + DoubleToString(c)+"=0";
    }

    inline bool has_dot(Dot *d) {
        switch (type) {
            case 'R':
                return d->onray(s, t);
            case 'S':
                return d->onsegment(s, t);
        }
        return true;
    }

    // 在比较时只看其一般式而不看类型，两条线相等只能证明共线！！！
    bool operator==(const Line &l) const {
        return equals(a, l.a) && equals(b, l.b) && equals(c, l.c);
    }

    bool operator<(const Line &l) const {
        return !equals(a, l.a)? a < l.a: (!equals(b, l.b)? b < l.b: c < l.c);
    }
};

class Circle : public Diagram {
public:
    double x, y, r, x2, y2, r2;

    Circle(double ax, double ay, double ar);

    string tostring() override {
        return "C:" + Dot(x, y).tostring() + DoubleToString(r);
    }

    double distance(Line *l);

    void intersect(Line *l);

    void intersect(Circle *c);

    bool operator==(const Circle &l) const {
        return equals(x, l.x) && equals(y, l.y) && equals(r, l.r);
    }

    bool operator<(const Circle &l) const {
        return !equals(x, l.x)? x < l.x:(!equals(y, l.y)? y < l.y: r < l.r);
    }
};

class Node {
public:
    Node *up;
    Node *down;
    Node *left;
    Node *right;
    Diagram *diagram;
    Dot *dot;

    Node() : diagram(nullptr), dot(nullptr), left(this), right(this), down(this), up(this) {}
    Node(Diagram *d) : Node() { diagram = d; }
    Node(Dot *d) : Node() { dot = d; }
    Node(Diagram *d1, Dot *d2) : Node() { diagram = d1; dot = d2; }


    inline void right_insert(Node *node) {
        node->left = this;
        node->right = right;
        right->left = node;
        right = node;
    }

    inline void left_insert(Node *node) {
        node->right = this;
        node->left = left;
        left->right = node;
        left = node;
    }

    inline void down_insert(Node *node) {
        node->up = this;
        node->down = down;
        down->up = node;
        down = node;
    }

    inline void up_insert(Node *node) {
        node->down = this;
        node->up = up;
        up->down = node;
        up = node;
    }

    void remove();

    void invalify();
};

extern map<Dot, Node*> point_map;
extern set<Circle> circles;
extern set<Line> lines;

void add_diagram(char L, int a, int b, int c, int d);
void sub_diagram(char L, int a, int b, int c, int d);
void handle_error(const string& msg);

#endif //INTERSECTPROJECT_LINE_H
