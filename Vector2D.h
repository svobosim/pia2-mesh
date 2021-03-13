#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "Mesh.h"

class Vector2D {
public:
    Vector2D(double _x, double _y) : x(_x), y(_y) {};
    Vector2D(Point const& u, Point const& v) : x(v.x - u.x), y(v.y - u.y) {};
    friend Vector2D operator+(Vector2D const& v, Vector2D const& u);
    friend Vector2D operator-(Vector2D const& v, Vector2D const& u);
    //friend Vector2D operator*(double a, Vector2D const& u);
    //friend Vector2D operator*(Vector2D const& u, double a);
    //friend Vector2D operator/(Vector2D const& u, double a);
    friend double dot(Vector2D const& v, Vector2D const& u);
    double x, y;
};

#endif // VECTOR2D_H
