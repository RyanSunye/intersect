#ifndef DLL_H
#define DLL_H

#include "dll_global.h"
#include <math.h>
#include<time.h>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <set>
using namespace std;
const double eps = 1e-10;
int dcmp(double x);
struct Point {
    double x, y;

    Point(double x = 0, double y = 0) :x(x), y(y) {

    }

    bool operator < (const Point& b)const {
        return dcmp(x - b.x) < 0 || (!dcmp(x - b.x) && dcmp(y - b.y) < 0);
    }
};
struct PointHashFunc
{
    size_t operator()(const Point& p) const noexcept
    {
        return std::hash<double>{}(p.x) ^ (std::hash<double>{}(p.y) << 1);
    }
};

typedef Point Vector;
Vector operator + (const Vector& A, const Vector& B);
Vector operator - (const Vector& A, const Vector& B);
double operator * (const Vector& A, const Vector& B);
Vector operator * (const Vector& A, const double& p);
bool operator == (const Vector& A, const Vector& B);
double operator ^ (const Vector& A, const Vector& B);


struct Line {
    Point p;
    Vector v;
    string type;
    Line(Point p, Vector v, string type) :p(p), v(v), type(type) {

    }

    Point point(double t)const {
        return p + v * t;
    }

    bool isOnLine(const Point& b)const {
        double t = -1;
        if (type == "R") { //ÉäÏß
            if (v.x == 0) {
                t = (b - p).y / v.y;
            }
            else if (v.y == 0) {
                t = (b - p).x / v.x;
            }
            else {
                t = (b - p).x / v.x;
            }
            return t > 0;
        }
        else if (type == "S") {//Ïß¶Î
            if (v.x == 0) {
                t = (b - p).y / v.y;
            }
            else if (v.y == 0) {
                t = (b - p).x / v.x;
            }
            else {
                t = (b - p).x / v.x;
            }
            return t >= 0 && t <= 1;
        }
        return true;
    }

};

struct Circle {
    Point c;
    double r;
    Circle(Point c, double r) :c(c), r(r) {

    }
    Point point(double a)const {
        return Point(c.x + r * cos(a), c.y + r * sin(a));
    }
};

double Length(Vector x);
double angle(Vector vec);

class DLL_EXPORT Dll
{
public:
    Dll();
    void add_diagram(char T, int x1, int y1, int x2, int y2);
    void sub_diagram(char T, int a, int b, int c, int d);

    void lineIntersectLine_ui(const Line& l1, const Line& l2);
    void lineIntersectCircle_ui(const Line& L, const Circle& C);
    void circleIntersectCircle_ui(const Circle& c1, const Circle& c2);
    void calPoints();

    set<pair<double, double>> uiPoints;
    vector<Line> lineVector;
    vector<Circle> circleVector;
};




#endif // DLL_H
