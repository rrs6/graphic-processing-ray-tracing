#include "Scene.hpp"
#include "../core/Vector.hpp"
#include "../utils/VectorOperations.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <math.h>
#include "../obj/Sphere.hpp"
#include "Camera.hpp"

Scene::Scene(const Camera& camera, int width, int height, double distance)
    : camera(camera), width(width), height(height), screenDistance(distance) {}

Camera Scene::getCamera() const { return camera; }

int Scene::getWidth() const { return width; }

int Scene::getHeight() const { return height; }

double Scene::getScreenDistance() const { return screenDistance; }

void Scene::setCamera(const Camera& camera) { this->camera = camera; }

void Scene::setWidth(int width) { this->width = width; }

void Scene::setHeight(int height) { this->height = height; }

void Scene::setScreenDistance(double distance) { screenDistance = distance; }


void Scene::render() {
    Sphere circle1 = Sphere(Point(0, 0, -80), 2, RGB(24, 89, 199));
    Sphere circle2 = Sphere(Point(0, 0, -90), 4, RGB(255, 255, 255));
    Camera cam = getCamera();
    Vector front = cam.getFront();
    Vector up = cam.getUp();
    Vector right = cam.getW();

    Point screenO = Point(
        cam.getO().getX() + (front.getX() * screenDistance),
        cam.getO().getY() + (front.getY() * screenDistance),
        cam.getO().getZ() + (front.getZ() * screenDistance)
    );
    double aspectRatio = static_cast<double>(width) / height;
    std::vector<std::vector<RGB>> image(height, std::vector<RGB>(width, RGB(255, 255, 255)));

     for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double u = ((static_cast<double>(i) / (width - 1)) - 0.5) * 2 * aspectRatio;
            double v = ((static_cast<double>(j) / (height - 1)) - 0.5) * 2;

            Vector offset = VectorOperations::escalar(right, u);
            offset = VectorOperations::soma(offset, VectorOperations::escalar(up, v));

            Point screenPoint = Point(
                screenO.getX() + offset.getX(),
                screenO.getY() + offset.getY(),
                screenO.getZ() + offset.getZ()
            );

            image[j][i] = checkIntersections(std::vector<Sphere>{circle1, circle2}, screenPoint);
        }
    }
    std::ofstream out("output.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            RGB& pixel = image[j][i];
            out << int(pixel.r) << ' ' << int(pixel.g) << ' ' << int(pixel.b) << '\n';
        }
    }
    out.close();
}
RGB Scene::checkIntersections(std::vector<Sphere> &circles, Point &screenPoint)
{
    std::set<std::pair<double, RGB>, bool(*)(const std::pair<double, RGB>&, const std::pair<double, RGB>&)> hits(
    [](const std::pair<double, RGB>& a, const std::pair<double, RGB>& b) {
        return a.first < b.first;
    }
);
    for(int i = 0; i < circles.size(); i++) {
        Vector ray = Vector(
               screenPoint.getX() - getCamera().getO().getX(),
               screenPoint.getY() - getCamera().getO().getY(),
               screenPoint.getZ() - getCamera().getO().getZ()
            );

        Vector oc = Vector(
            getCamera().getO().getX() - circles[i].getO().getX(),
            getCamera().getO().getY() - circles[i].getO().getY(),
            getCamera().getO().getZ() - circles[i].getO().getZ()
        );

        double a = VectorOperations::dot(ray, ray);
        double b = 2.0 * VectorOperations::dot(oc, ray);
        double c = VectorOperations::dot(oc, oc) - (circles[i].getR() * circles[i].getR());

        const double delta = (b * b) - (4 * a * c);
        if(delta >= 0) {
            const double t1 = (-b + sqrt(delta))/(2*a);
            const double t2 = (-b - sqrt(delta))/(2*a);
            if(t1>0 && t2>0)
                hits.insert({std::min(t1, t2), circles[i].getColor()});
            else if(t1>0)
                hits.insert({t1, circles[i].getColor()});
            else if(t2>0)
                hits.insert({t2, circles[i].getColor()});
        }
    }
    return (hits.empty() ? RGB(0,0,0) : hits.begin()->second);
}
