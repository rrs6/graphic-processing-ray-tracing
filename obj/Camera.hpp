#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../core/Point.hpp"
#include "../core/Vector.hpp"

class Camera {
private:
    Point o;
    Point target;
    Vector up;
    Vector front;
    Vector w;
    double d;

public:
    Camera(const Point& o, const Point& target, const Vector& up, const double d);
    
    Point getO() const;
    Point getTarget() const;
    Vector getUp() const;
    Vector getW() const;
    Vector getFront() const;
    double getD() const;

    void setO(const Point& origem);
    void setTarget(const Point& alvo);
    void setUp(const Vector& up);
    void setFront(const Vector& front);
    void setD(const double d);
};

#endif