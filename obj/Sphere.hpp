#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../core/Point.hpp"
#include "../utils/RGB.hpp"
#include <array>

using namespace std;

class Sphere
{
private:
    Point o;
    double r;
    RGB color;
    array<double, 6> props;
public:
    Sphere(const Point &o, const double r, const RGB &color, const array<double, 6> &props);

    Point getO() const;
    double getR() const;
    RGB getColor() const;
    void transform(const double (&m)[4][4]);
    array<double, 6> getProps() const;
};

#endif