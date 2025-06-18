#include "Point.hpp"

// Construtores
Point::Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
Point::Point() : x(0), y(0), z(0), w(1) {}

// Getters
double Point::getX() const { return x; }
double Point::getY() const { return y; }
double Point::getZ() const { return z; }
double Point::getW() const { return w; }

// Setters
void Point::setX(double x) { this->x = x; }
void Point::setY(double y) { this->y = y; }
void Point::setZ(double z) { this->z = z; }
void Point::setW(double w) { this->w = w; }
