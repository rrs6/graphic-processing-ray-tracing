#include "Vector.hpp"

// Implementação dos métodos da classe Vector
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}
Vector::Vector() : x(0), y(0), z(0) {}

double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getZ() const { return z; }

void Vector::setX(double x) { this->x = x; }
void Vector::setY(double y) { this->y = y; }
void Vector::setZ(double z) { this->z = z; }