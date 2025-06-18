#include "Plan.hpp"

Plan::Plan(const Point& p, const Vector &normal, const RGB &color) : p(p), normal(normal), color(color) {}

Point Plan::getP() const { return p; }

Vector Plan::getNormal() const {return normal; }

RGB Plan::getColor() const {return color; }