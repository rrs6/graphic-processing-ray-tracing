#ifndef PLAN_HPP
#define PLAN_HPP

#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../utils/RGB.hpp"
#include <array>
using namespace std;

class Plan
{
private:
    Point p;
    Vector normal;
    RGB color;
    array<double, 6> props;

public:
    Plan(const Point &o, const Vector &normal, const RGB &color, const array<double, 6> &props);

    Point getP() const;
    Vector getNormal() const;
    RGB getColor() const;
    void transform(const double (&m)[4][4]);
    array<double, 6> getProps() const;
};

#endif