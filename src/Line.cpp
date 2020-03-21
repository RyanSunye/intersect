//
// Created by HP on 2020/3/4->
//

#include <cmath>
#include "Line.h"
#include <stack>
#include <iostream>
#include <cassert>
#include <cstdarg>

#define REIGN 100000
#define INREIGN(t) ((t)<=REIGN && (t)>= -REIGN)

map<Dot, Node *> point_map;
Node *head = new Node();
vector<Diagram *> diagrams;
set<Circle> circles;
set<Line> lines;

void add_pair(Diagram *d1, Diagram *d2, Dot *dot);

void assert_reign(double x) {
    if (!INREIGN(x)) handle_error("parameter not in (-100000,100000)");
}

string DoubleToString(double value, unsigned int precision) {
    ostringstream out;
    if (precision > 0) out.precision(precision);
    out << value;
    return out.str();
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

bool Line::parallel(Line *l) {
    return a * l->b == l->a * b;
}

void Line::intersect(Line *l) {
    try {
        Dot *d = intersect0(l);
        if (!has_dot(d) || !l->has_dot(d)) return;
        add_pair(this, l, d);
    } catch (exception e) {}
}

Dot *Line::intersect0(Line *l) {
    return new Dot(Dot(b * l->c - l->b * c, l->a * c - a * l->c) / (a * l->b - l->a * b));
}

Line::Line(double aa, double ab, double ac) {
    a = aa;
    b = ab;
    c = ac;
    type = 'L';
}


Line::Line(int x0, int y0, int x1, int y1, char ty) {
    assert_reign(x0);
    assert_reign(y0);
    assert_reign(x1);
    assert_reign(y1);
    // 转换成一般式，并保证互质，同时a要非负，a为0，b要非负
    double divider = gcd(gcd(abs(y1 - y0), abs(x0 - x1)), abs(x1 * y0 - x0 * y1));
    if (equals(divider, 0)) {
        cout<<x0<<" "<<y0<<" "<<x1<<" "<<y1<<endl;
        handle_error("Line::Line\ttwo dots coincide!");
    }
    a = (y1 - y0) / divider;
    b = (x0 - x1) / divider;
    c = (x1 * y0 - x0 * y1) / divider;
    if (a < 0 || (a == 0 && b < 0)) {
        a = -a;
        b = -b;
        c = -c;
    }
    s = new Dot(x0, y0);
    t = new Dot(x1, y1);
    type = ty;

    // 更新端点值，以便后续作图
    if (type == 'L') {
        if (equals(b, 0)) {
            s = new Dot(-c / a, -REIGN);
            t = new Dot(-c / a, REIGN);
        } else if (equals(a, 0)) {
            s = new Dot(-REIGN, -c / b);
            t = new Dot(REIGN, -c / b);
        } else {
            set<Dot> dot_stack;
            if (INREIGN((-c + a * REIGN) / b)) dot_stack.insert(Dot(-REIGN, (-c + a * REIGN) / b));
            if (INREIGN((-c + b * REIGN) / a)) dot_stack.insert(Dot((-c + b * REIGN) / a, -REIGN));
            if (INREIGN((-c - a * REIGN) / b)) dot_stack.insert(Dot(REIGN, (-c - a * REIGN) / b));
            if (INREIGN((-c - b * REIGN) / a)) dot_stack.insert(Dot((-c - b * REIGN) / a, REIGN));
            auto it = dot_stack.begin();
            s = new Dot((*it).first, (*it).second);
            it++;
            t = new Dot((*it).first, (*it).second);
        }
    } else if (type == 'R') {
        if (equals(b, 0)) {
            t->second = s->second < t->second ? REIGN : -REIGN;
        } else if (equals(a, 0)) {
            t->first = s->first < t->first ? REIGN : -REIGN;
        } else {
            Dot *dot = new Dot(-REIGN, (-c + a * REIGN) / b);
            if (dot->onray(s, t)) {
                t = dot;
                return;
            }
            dot = new Dot((-c + b * REIGN) / a, -REIGN);
            if (dot->onray(s, t)) {
                t = dot;
                return;
            }
            dot = new Dot(REIGN, (-c - a * REIGN) / b);
            if (dot->onray(s, t)) {
                t = dot;
                return;
            }
            dot = new Dot((-c - b * REIGN) / a, REIGN);
            if (dot->onray(s, t)) {
                t = dot;
                return;
            }
        }
    }
}

double Circle::distance(Line *l) {
    return fabs(l->a * x + l->b * y + l->c) / Dot(l->a, l->b).abs();
}

void Circle::intersect(Line *l) {
    double d = distance(l);
    if (d > r) return;
    Line ll(l->b, -l->a, l->a * y - l->b * x);
    Dot *dot = ll.intersect0(l);
    if (d == r) {
        add_pair(this, l, dot);
        return;
    }
    double t = sqrt(r * r - d * d);
    Dot direction(l->b, -l->a);
    direction.unify();
    Dot *d1 = new Dot(*dot + direction * t);
    Dot *d2 = new Dot(*dot - direction * t);
    if (l->has_dot(d1)) add_pair(this, l, d1);
    if (l->has_dot(d2)) add_pair(this, l, d2);
    delete dot;    // 释放内存，direction的析构自动调用
}

void Circle::intersect(Circle *c) {
    double subs1 = x2 - 2 * x * c->x + c->x2 + y2 - 2 * y * c->y + c->y2;
    if (subs1 <= 0) return;

    double subs2 = -r2 * x + r2 * c->x + c->r2 * x - c->r2 * c->x + x2 * x - x2 * c->x - x * c->x2 + x * y2 -
                   2 * x * y * c->y + x * c->y2 + c->x2 * c->x + c->x * y2 - 2 * c->x * y * c->y + c->x * c->y2;
    double subs3 = -r2 * y + r2 * c->y + c->r2 * y - c->r2 * c->y + x2 * y + x2 * c->y - 2 * x * c->x * y -
                   2 * x * c->x * c->y + c->x2 * y + c->x2 * c->y + y2 * y - y2 * c->y - y * c->y2 + c->y2 * c->y;
    double sigma = sqrt((r2 + 2 * r * c->r + c->r2 - x2 + 2 * x * c->x - c->x2 - y2 + 2 * y * c->y - c->y2) *
                        (-r2 + 2 * r * c->r - c->r2 + subs1));

    Dot dot1(subs2, subs3), dot2(y - c->y, c->x - x);
    Dot *d1 = new Dot((dot1 + dot2 * sigma) / (2 * subs1));
    Dot *d2 = new Dot((dot1 - dot2 * sigma) / (2 * subs1));
    add_pair(this, c, d1);
    add_pair(this, c, d2);
}

void Diagram::intersect(Diagram *diagram) {
    if (typeid(*this) == typeid(Circle)) {
        if (typeid(*diagram) == typeid(Circle)) ((Circle *) this)->intersect((Circle *) diagram);
        else ((Circle *) this)->intersect((Line *) diagram);
    } else {
        if (typeid(*diagram) == typeid(Circle)) ((Circle *) diagram)->intersect((Line *) this);
        else ((Line *) this)->intersect((Line *) diagram);
    }
}

void handle_error(const string &msg) {
    cout << msg;
    exit(1);
}

Node *get_node(Diagram *d) {
    if (d->node == nullptr) {
        d->node = new Node(d);
        head->right_insert(d->node);
    }
    return d->node;
}

Node *get_node(Dot *d) {
    if (point_map.count(*d) == 0) {
        if (DEBUG) cout << "creating dot node" + d->tostring() << endl;
        Node *node = new Node(d);
        head->up_insert(node);
        point_map[*d] = node;
    }
    return point_map[*d];
}

void add_pair(Diagram *d1, Diagram *d2, Dot *dot) {
    Node *n = get_node(dot);
    Node *d = n->right;
    bool valid1 = true, valid2 = true;
    while (d != n) {
        if (d->diagram == d1) valid1 = false;
        if (d->diagram == d2) valid2 = false;
        d = d->right;
    }

    if (valid1) {
        // 在函数中创建的对象都会释放，必须创建指针对象!!!!!!!!!!!!
        Node *p = new Node(d1, dot);
        n->left_insert(p);
        get_node(d1)->up_insert(p);
    }
    if (valid2) {
        Node *p = new Node(d2, dot);
        n->left_insert(p);
        get_node(d2)->up_insert(p);
    }
}

void add_diagram(char L, int a, int b, int c, int d) {
    Diagram *diag;
    if (L == 'C') {
        // 判断是否重复
        auto *circle = new Circle(a, b, c);
        if (circles.count(*circle) == 1) handle_error("add_diagram\trepeated circles");
        circles.insert(*circle);
        diag = circle;
    } else {
        auto *line = new Line(a, b, c, d, L);
        if (lines.count(*line) == 1) handle_error("add_diagram\trepeated lines or collinear lines");
        lines.insert(*line);
        diag = line;
    }
    if (DEBUG) cout << diag->tostring() + "start calculate!" << endl;
    for (auto it: diagrams) diag->intersect(it);
    diagrams.emplace_back(diag);
}

Diagram *get_diagram(char L, int a, int b, int c, int d) {
    string s(1, L);
    for (auto it: diagrams) {
        if (L == 'C') {
            if (it->tostring() == s + ":" + Dot(a, b).tostring() + DoubleToString(c)) return it;
        } else {
            if (it->tostring() == s + ":" + Dot(a, b).tostring() + Dot(c, d).tostring()) return it;
        }
    }
    handle_error("required diagram not found!");
    return nullptr;
}

void sub_diagram(char L, int a, int b, int c, int d) {
    Diagram *dd = get_diagram(L, a, b, c, d);
    if (typeid(*dd) == typeid(Circle)) {
        circles.erase(*((Circle *) dd));
    } else {
        lines.erase(*((Line *) dd));
    }
    get_node(dd)->invalify();
    delete dd;
}

void Node::invalify() {
    if (dot == nullptr) {
        if (DEBUG) cout << "deleting " << diagram->tostring() << endl;
        //该节点是Diagram头结点
        Node *d = down;
        Node *dd = d->down;
        while (d != this) {
            d->invalify();
            d = dd;
            dd = d->down;
        }
    } else {
        //该节点是中间结点
        if (DEBUG) cout << "deleting" << dot->tostring() << endl;
        if ((right->diagram == nullptr && left->left == right)
            || (left->diagram == nullptr && right->right == left)) {
            left->remove();
            right->remove();
        }
    }
    remove();
}

void Node::remove() {
    up->down = down;
    down->up = up;
    left->right = right;
    right->left = left;
    if (diagram == nullptr) point_map.erase(*dot);//删除对应节点
    delete this; //显式释放内存！防止内存泄露
}

Circle::Circle(double ax, double ay, double ar) {
    assert_reign(ax);
    assert_reign(ay);
    if(ar<=0) handle_error("Circle: r<0!");
    x = ax;
    y = ay;
    r = ar;
    x2 = x * x;
    y2 = y * y;
    r2 = r * r;

}
