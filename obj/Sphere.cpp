#include "../core/Point.hpp"
#include "Sphere.hpp"
#include <array>
#include "Colormap.cpp"

using namespace std;

Sphere::Sphere(const Point& o, const double r, const RGB &color, const MaterialProperties &props) : o(o), r(r), color(color), props(props) {}

Point Sphere::getO() const { return o; }

double Sphere::getR() const {return r; }

RGB Sphere::getColor() const {return color; }

void Sphere::transform(const double (&m)[4][4]) {
    this->o = Point(
        (this->o.getX() * m[0][0]) + (this->o.getY() * m[0][1]) + (this->o.getZ() * m[0][2]) + m[0][3],
        (this->o.getX() * m[1][0]) + (this->o.getY() * m[1][1]) + (this->o.getZ() * m[1][2]) + m[1][3],
        (this->o.getX() * m[2][0]) + (this->o.getY() * m[2][1]) + (this->o.getZ() * m[2][2]) + m[2][3]
    );
}

MaterialProperties Sphere::getProps() const {
    return this->props;
}