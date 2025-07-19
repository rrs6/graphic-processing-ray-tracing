#include "Light.hpp"

Light::Light(const Point &o, const RGB &color) : o(o), color(color) {}

RGB Light::getColor() const {
    return this->color;
}

Point Light::getO() const {
    return this->o;
}