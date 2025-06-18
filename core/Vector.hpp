#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector {
private:
    double x, y, z;

public:
    Vector(double x, double y, double z);
    Vector();

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);
};

#endif
