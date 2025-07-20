#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../core/Point.hpp"
#include "../utils/RGB.hpp"
#include <array>
#include "Colormap.cpp"

using namespace std;

class Sphere
{
private:
    Point o;
    double r;
    RGB color;
    MaterialProperties props;
public:
    Sphere(const Point &o, const double r, const RGB &color, const MaterialProperties &props);

    Point getO() const;
    double getR() const;
    RGB getColor() const;
    void transform(const double (&m)[4][4]);
    MaterialProperties getProps() const;
};

#endif