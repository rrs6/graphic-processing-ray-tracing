#ifndef POINT_HPP
#define POINT_HPP

class Point {
private:
    double x, y, z, w;

public:
    Point(double x, double y, double z, double w = 1.0);
    Point();

    double getX() const;
    double getY() const;
    double getZ() const;
    double getW() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setW(double w);
};

#endif
