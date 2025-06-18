#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../core/Point.hpp"
#include "../utils/RGB.hpp"

class Sphere
{
private:
    Point o;
    double r;
    RGB color;
public:
    Sphere(const Point &o, const double r, const RGB &color);

    Point getO() const;
    double getR() const;
    RGB getColor() const;
};

#endif