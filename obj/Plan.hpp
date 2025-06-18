#ifndef PLAN_HPP
#define PLAN_HPP

#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../utils/RGB.hpp"

class Plan
{
private:
    Point p;
    Vector normal;
    RGB color;
public:
    Plan(const Point &o, const Vector &normal, const RGB &color);

    Point getP() const;
    Vector getNormal() const;
    RGB getColor() const;
};

#endif