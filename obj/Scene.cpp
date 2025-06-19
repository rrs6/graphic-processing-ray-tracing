#include "Scene.hpp"
#include "../obj/Plan.hpp"
#include "../obj/Sphere.hpp"
#include "../obj/Camera.hpp"
#include "../core/Vector.hpp"
#include "../utils/VectorOperations.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <math.h>

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
    Sphere s1 = Sphere(Point(2, -4.5, -2), 0.5, RGB(179, 51, 51));   // menor - vermelho
    Sphere s2 = Sphere(Point(0, -4, -2), 1, RGB(51, 179, 51));   // média - verde
    Sphere s3 = Sphere(Point(-3, -3.5, -2), 1.5, RGB(51, 51, 179));   // maior - azul

    Plan plan1 = Plan(Point(5, 0, 0),     Vector(-1, 0, 0),  RGB(51, 179, 51));   // parede direita (verde)
    Plan plan2 = Plan(Point(-5, 0, 0),    Vector(1, 0, 0),   RGB(179, 51, 51));   // parede esquerda (vermelha)
    Plan plan3 = Plan(Point(0, -5, 0),    Vector(0, 1, 0),   RGB(186, 186, 186)); // chão (branco)
    Plan plan4 = Plan(Point(0, 5, 0),     Vector(0, -1, 0),  RGB(186, 186, 186)); // teto (branco)
    Plan plan5 = Plan(Point(0, 0, -5),    Vector(0, 0, 1),   RGB(186, 186, 186)); // fundo (branco)
    Plan plan6 = Plan(Point(0, 0, 6),     Vector(0, 0, -1),  RGB(186, 186, 186)); // frente (branco)

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
            double v = ((static_cast<double>(j) / (height - 1)) - 0.5) * -2;

            Vector offset = VectorOperations::escalar(right, u);
            offset = VectorOperations::soma(offset, VectorOperations::escalar(up, v));

            Point screenPoint = Point(
                screenO.getX() + offset.getX(),
                screenO.getY() + offset.getY(),
                screenO.getZ() + offset.getZ()
            );

            image[j][i] = checkIntersections(std::vector<Sphere>{s1, s2, s3}, std::vector<Plan>{plan1, plan2, plan3, plan4, plan5, plan6}, screenPoint);
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
RGB Scene::checkIntersections(std::vector<Sphere> &spheres, std::vector<Plan> &planes, Point &screenPoint)
{
    std::set<std::pair<double, RGB>, bool(*)(const std::pair<double, RGB>&, const std::pair<double, RGB>&)> hits(
    [](const std::pair<double, RGB>& a, const std::pair<double, RGB>& b) {
        return a.first < b.first;
    }
);
    Vector ray = Vector(
        screenPoint.getX() - getCamera().getO().getX(),
        screenPoint.getY() - getCamera().getO().getY(),
        screenPoint.getZ() - getCamera().getO().getZ()
    );
    for(int i = 0; i < planes.size(); i++) {
        double t = VectorOperations::dot(planes[i].getNormal(), 
        Vector(
            planes[i].getP().getX() - getCamera().getO().getX(), 
            planes[i].getP().getY() - getCamera().getO().getY(),
            planes[i].getP().getZ() - getCamera().getO().getZ()
        )) / VectorOperations::dot(planes[i].getNormal(), ray);

        if(t > 0) {
            hits.insert({t, planes[i].getColor()});
        }
    }

    for(int i = 0; i < spheres.size(); i++) {

        Vector oc = Vector(
            getCamera().getO().getX() - spheres[i].getO().getX(),
            getCamera().getO().getY() - spheres[i].getO().getY(),
            getCamera().getO().getZ() - spheres[i].getO().getZ()
        );

        double a = VectorOperations::dot(ray, ray);
        double b = 2.0 * VectorOperations::dot(oc, ray);
        double c = VectorOperations::dot(oc, oc) - (spheres[i].getR() * spheres[i].getR());

        const double delta = (b * b) - (4 * a * c);
        if(delta >= 0) {
            const double t1 = (-b + sqrt(delta))/(2*a);
            const double t2 = (-b - sqrt(delta))/(2*a);
            if(t1>0 && t2>0)
                hits.insert({std::min(t1, t2), spheres[i].getColor()});
            else if(t1>0)
                hits.insert({t1, spheres[i].getColor()});
            else if(t2>0)
                hits.insert({t2, spheres[i].getColor()});
        }
    }
    return (hits.empty() ? RGB(0,0,0) : hits.begin()->second);
}
