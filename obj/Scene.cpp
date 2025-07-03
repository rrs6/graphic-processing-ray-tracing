#include "Scene.hpp"
#include "../obj/Plan.hpp"
#include "../obj/Sphere.hpp"
#include "../obj/Camera.hpp"
#include "../core/Vector.hpp"
#include "../obj/TriangleMesh.hpp"
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
    std::vector<Point> vertices = {
        Point(0,    1.25, 10),     // 0 - topo
        Point(1.785, 0,   10),     // 1 - direita
        Point(0,   -1.25, 10),     // 2 - base
        Point(-1.785, 0,  10)
    };

    TriangleMesh losango = TriangleMesh(
        vertices,
        {{0, 1, 2},
        {0, 2, 3}},
        {Vector(0, 0, 1), Vector(0, 0, 1)},
        RGB(255, 223, 0) // brown
    );


    Sphere s1 = Sphere(Point(0, 0, 10), 0.8, RGB(0, 39, 118));
    // Sphere s1 = Sphere(Point(2, -4.5, -2), 0.5, RGB(179, 51, 51));   // menor - vermelho
    // Sphere s2 = Sphere(Point(0, -4, -2), 1, RGB(51, 179, 51));   // média - verde
    // Sphere s3 = Sphere(Point(-3, -3.5, -2), 1.5, RGB(51, 51, 179));   // maior - azul

    Plan plan1 = Plan(Point(0, 0, 10), Vector(0, 0, 1), RGB(0, 148, 64));   // parede direita (verde)

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

            Vector offset = VectorOperations::scale(right, u);
            offset = VectorOperations::sum(offset, VectorOperations::scale(up, v));

            Point screenPoint = Point(
                screenO.getX() + offset.getX(),
                screenO.getY() + offset.getY(),
                screenO.getZ() + offset.getZ()
            );

            image[j][i] = checkIntersections(std::vector<Sphere>{s1}, std::vector<Plan>{plan1},
                std::vector<TriangleMesh>{losango}, screenPoint);
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
RGB Scene::checkIntersections(std::vector<Sphere> &spheres, std::vector<Plan> &planes, std::vector<TriangleMesh> &meshs, Point &screenPoint)
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

    for(int i = 0; i < meshs.size(); i++) {
        for(int j = 0; j < meshs[i].getMesh().size(); j++) {
            std::tuple<int,int,int> vertices = meshs[i].getMesh().at(j);
            
            Vector normal = meshs[i].getNormals().at(j);
            
            Point v0 = meshs[i].getVertices().at(std::get<0>(vertices));
            Point v1 = meshs[i].getVertices().at(std::get<1>(vertices));
            Point v2 = meshs[i].getVertices().at(std::get<2>(vertices));

            double t = VectorOperations::dot(normal, Vector(
                v1.getX() - getCamera().getO().getX(),
                v1.getY() - getCamera().getO().getY(),
                v1.getZ() - getCamera().getO().getZ()
            )) / VectorOperations::dot(normal, ray);

            if(t > 0) {
                Point trianglePlanePoint = Point(
                    getCamera().getO().getX() + VectorOperations::scale(ray, t).getX(),
                    getCamera().getO().getY() + VectorOperations::scale(ray, t).getY(),
                    getCamera().getO().getZ() + VectorOperations::scale(ray, t).getZ()
                );

                Vector v0v1 = Vector(v1.getX() - v0.getX(), v1.getY() - v0.getY(), v1.getZ() - v0.getZ());
                Vector v0v2 = Vector(v2.getX() - v0.getX(), v2.getY() - v0.getY(), v2.getZ() - v0.getZ());
                Vector v0p = Vector(trianglePlanePoint.getX() - v0.getX(), trianglePlanePoint.getY() - v0.getY(), trianglePlanePoint.getZ() - v0.getZ());

                double d00 = VectorOperations::dot(v0v1, v0v1);
                double d01 = VectorOperations::dot(v0v1, v0v2);
                double d11 = VectorOperations::dot(v0v2, v0v2);
                double d20 = VectorOperations::dot(v0p,  v0v1);
                double d21 = VectorOperations::dot(v0p,  v0v2);

                double denom = d00 * d11 - d01 * d01;
                double v = (d11 * d20 - d01 * d21) / denom;
                double w = (d00 * d21 - d01 * d20) / denom;
                double u = 1.0 - v - w;

                if(u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1) {
                    hits.insert({t, meshs[i].getColor()});
                }
            }

        }
    }

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
