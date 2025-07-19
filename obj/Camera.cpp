#include <cmath>
#include "Camera.hpp"
#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../utils/VectorOperations.hpp"

Camera::Camera(const Point& o, const Point& target, const Vector& up, double d) : o(o), target(target), up(up), d(d){
    front = Vector(target.getX() - o.getX(),
                target.getY()- o.getY(),
                target.getZ() - o.getZ());
    front = VectorOperations::normalize(front);
    w = VectorOperations::normalize(VectorOperations::crossProduct(front, up));
    this->up = VectorOperations::normalize(VectorOperations::crossProduct(w, front));
}

Point Camera::getO() const { return o; }
Point Camera::getTarget() const { return target; }
Vector Camera::getFront() const {return front; }
Vector Camera::getUp() const { return up; }
Vector Camera::getW() const { return w; }
double Camera::getD() const { return d; }

void Camera::setO(const Point& o) { this->o = o; }
void Camera::setTarget(const Point& target) { this->target = target; }
void Camera::setUp(const Vector& up) { this->up = up; }
void Camera::setD(const double d) { this->d = d; }
void Camera::transform(const double (&m)[4][4]) {
    this->o = Point(
        (this->o.getX() * m[0][0]) + (this->o.getY() * m[0][1]) + (this->o.getZ() * m[0][2]) + m[0][3],
        (this->o.getX() * m[1][0]) + (this->o.getY() * m[1][1]) + (this->o.getZ() * m[1][2]) + m[1][3],
        (this->o.getX() * m[2][0]) + (this->o.getY() * m[2][1]) + (this->o.getZ() * m[2][2]) + m[2][3]
    );

    this->front = Vector(
        (this->front.getX() * m[0][0]) + (this->front.getY() * m[0][1]) + (this->front.getZ() * m[0][2]),
        (this->front.getX() * m[1][0]) + (this->front.getY() * m[1][1]) + (this->front.getZ() * m[1][2]),
        (this->front.getX() * m[2][0]) + (this->front.getY() * m[2][1]) + (this->front.getZ() * m[2][2])
    );

    this->up = Vector(
        (this->up.getX() * m[0][0]) + (this->up.getY() * m[0][1]) + (this->up.getZ() * m[0][2]),
        (this->up.getX() * m[1][0]) + (this->up.getY() * m[1][1]) + (this->up.getZ() * m[1][2]),
        (this->up.getX() * m[2][0]) + (this->up.getY() * m[2][1]) + (this->up.getZ() * m[2][2])
    );

    this->w = Vector(
        (this->w.getX() * m[0][0]) + (this->w.getY() * m[0][1]) + (this->w.getZ() * m[0][2]),
        (this->w.getX() * m[1][0]) + (this->w.getY() * m[1][1]) + (this->w.getZ() * m[1][2]),
        (this->w.getX() * m[2][0]) + (this->w.getY() * m[2][1]) + (this->w.getZ() * m[2][2])
    );
}