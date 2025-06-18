#include <cmath>
#include "../core/Vector.hpp"
#include "VectorOperations.hpp"

Vector VectorOperations::soma(const Vector& a, const Vector& b) {
    return Vector(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

Vector VectorOperations::escalar(const Vector& v, double k) {
    return Vector(v.getX() * k, v.getY() * k, v.getZ() * k);
}

Vector VectorOperations::produtoVetorial(const Vector& a, const Vector& b) {
    double x = a.getY() * b.getZ() - a.getZ() * b.getY();
    double y = a.getZ() * b.getX() - a.getX() * b.getZ();
    double z = a.getX() * b.getY() - a.getY() * b.getX();
    return Vector(x, y, z);
}

double VectorOperations::magnitude(const Vector& a) {
    return std::sqrt(a.getX() * a.getX() + a.getY() * a.getY() + a.getZ() * a.getZ());
}

Vector VectorOperations::dirChange(const Vector& a) {
    return escalar(a, -1);
}

Vector VectorOperations::normalizar(const Vector& v) {
    double mag = magnitude(v);
    if (mag == 0) {
        return Vector(0, 0, 0);
    }
    return escalar(v, 1.0 / mag);
}

double VectorOperations::dot(const Vector& a, const Vector& b) {
    return a.getX() * b.getX() +
           a.getY() * b.getY() +
           a.getZ() * b.getZ();
}
