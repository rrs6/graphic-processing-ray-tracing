#include <iostream>

using namespace std;

class Point {
    private:
        double x, y, z, w;

    public:
        Point(double x, double y, double z, double w = 1.0) : x(x), y(y), z(z), w(w) {} //More efficient than Default constructor
        Point() : x(0), y(0), z(0), w(1) {}

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }

    double getZ() {
        return z;
    }

    double getW() {
        return w;
    }

    double setX(double x) {
        return this->x = x;
    }

    double setY(double y) {
        return this->y = y;
    }

    double setZ(double z) {
        return this->z = z;
    }

    double setW(double w) {
        return this->w = w;
    }
};