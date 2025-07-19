#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../core/Point.hpp"
#include "../utils/RGB.hpp"

class Light {
private:
    Point o;
    RGB color;

public:
    Light(const Point& o, const RGB &color);
    
    Point getO() const;
    RGB getColor() const;
};

#endif