#include "Plan.hpp"

Plan::Plan(const Point& p, const Vector &normal, const RGB &color) : p(p), normal(normal), color(color) {}

Point Plan::getP() const { return p; }

Vector Plan::getNormal() const {return normal; }

RGB Plan::getColor() const {return color; }

void Plan::transform(const double (&m)[4][4]) {
    this->p = Point(
        (this->p.getX() * m[0][0]) + (this->p.getY() * m[0][1]) + (this->p.getZ() * m[0][2]) + m[0][3],
        (this->p.getX() * m[1][0]) + (this->p.getY() * m[1][1]) + (this->p.getZ() * m[1][2]) + m[1][3],
        (this->p.getX() * m[2][0]) + (this->p.getY() * m[2][1]) + (this->p.getZ() * m[2][2]) + m[2][3]
    );
    this->normal = Vector(
        (this->normal.getX() * m[0][0]) + (this->normal.getY() * m[0][1]) + (this->normal.getZ() * m[0][2]),
        (this->normal.getX() * m[1][0]) + (this->normal.getY() * m[1][1]) + (this->normal.getZ() * m[1][2]),
        (this->normal.getX() * m[2][0]) + (this->normal.getY() * m[2][1]) + (this->normal.getZ() * m[2][2])
    );
}