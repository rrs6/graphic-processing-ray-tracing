#include <cmath>
#include "Camera.hpp"
#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../utils/VectorOperations.hpp"

Camera::Camera(const Point& o, const Point& target, const Vector& up, double d) : o(o), target(target), up(up), d(d){
    front = Vector(target.getX() - o.getX(),
                target.getY()- o.getY(),
                target.getZ() - o.getZ());
    front = VectorOperations::normalizar(front);
    w = VectorOperations::normalizar(VectorOperations::produtoVetorial(front, up));
    this->up = VectorOperations::normalizar(VectorOperations::produtoVetorial(w, front));
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
