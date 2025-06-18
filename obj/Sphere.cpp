#include "../core/Point.hpp"
#include "Sphere.hpp"

Sphere::Sphere(const Point& o, const double r, const RGB &color) : o(o), r(r), color(color) {}

Point Sphere::getO() const { return o; }

double Sphere::getR() const {return r; }

RGB Sphere::getColor() const {return color; }