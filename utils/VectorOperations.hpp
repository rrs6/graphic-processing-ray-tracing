#ifndef VETOR_OPS_HPP
#define VETOR_OPS_HPP

#include "../core/Vector.hpp"

class VectorOperations {
public:
    static Vector sum(const Vector& a, const Vector& b);
    static Vector scale(const Vector& v, double k);
    static Vector crossProduct(const Vector& a, const Vector& b);
    static double magnitude(const Vector& a);
    static Vector dirChange(const Vector& a);
    static Vector normalize(const Vector& v);
    static double dot(const Vector& a, const Vector& b);
};

#endif
