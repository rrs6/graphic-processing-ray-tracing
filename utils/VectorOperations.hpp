#ifndef VETOR_OPS_HPP
#define VETOR_OPS_HPP

#include "../core/Vector.hpp"

class VectorOperations {
public:
    static Vector soma(const Vector& a, const Vector& b);
    static Vector escalar(const Vector& v, double k);
    static Vector produtoVetorial(const Vector& a, const Vector& b);
    static double magnitude(const Vector& a);
    static Vector dirChange(const Vector& a);
    static Vector normalizar(const Vector& v);
    static double dot(const Vector& a, const Vector& b);
};

#endif
