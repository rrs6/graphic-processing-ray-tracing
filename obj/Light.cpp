#include "Light.hpp"

Light::Light(const Point &o, const RGB &color) : o(o), color(color) {}

RGB Light::getColor() {
    return this->color;
}

Point Light::getO() {
    return this->o;
}