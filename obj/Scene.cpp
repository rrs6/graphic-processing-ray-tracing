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
#include <array>
#include <tuple>
#include <math.h>

using namespace std;

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
    double m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    double m1[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    vector<Light> lights = {
        Light(Point(-5, 2, 30), RGB(200, 0, 0))
    };

    vector<Point> vertices = {
        Point(0,    1.25, 10),    // 0 - topo
        Point(1.785, 0,   10),     // 1 - direita
        Point(0,   -1.25, 10),     // 2 - base
        Point(-1.785, 0,  10)
    };

    TriangleMesh losango = TriangleMesh(
        vertices,
        {{0, 1, 2},
        {0, 2, 3}},
        {Vector(0, 0, 1), Vector(0, 0, 1)},
        RGB(255, 223, 0),
        {0, 0.4, 0.5, 30, 0, 0}
    );


    Sphere s1 = Sphere(Point(0, 0, 40), 3, RGB(0, 0, 0), {0, 0.8, 0.9, 20, 0, 0});
    //Plan plan1 = Plan(Point(0, 3, 10), Vector(0, 2, 0), RGB(0, 148, 64), {0, 0.4, 0.5, 30, 0, 0});
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
    vector<vector<RGB>> image(height, vector<RGB>(width, RGB(255, 255, 255)));

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

            image[j][i] = checkIntersections(vector<Sphere>{s1}, vector<Plan>{},
                vector<TriangleMesh>{}, screenPoint, lights);
        }
    }
    ofstream out("output.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            RGB& pixel = image[j][i];
            out << int(pixel.r) << ' ' << int(pixel.g) << ' ' << int(pixel.b) << '\n';
        }
    }
    out.close();
}
RGB Scene::checkIntersections(vector<Sphere> &spheres, vector<Plan> &planes, vector<TriangleMesh> &meshs, Point &screenPoint, vector<Light> &lights)
{
    set<tuple<double, RGB, array<double, 6>, Vector, Point>, bool(*)(const tuple<double, RGB, array<double, 6>, Vector, Point>&, const tuple<double, RGB, array<double, 6>, Vector, Point>&)> hits(
    [](const tuple<double, RGB, array<double, 6>, Vector, Point>& a, const tuple<double, RGB, array<double, 6>, Vector, Point>& b) {
        return get<0>(a) < get<0>(b);
    }
);
    Vector ray = Vector(
        screenPoint.getX() - getCamera().getO().getX(),
        screenPoint.getY() - getCamera().getO().getY(),
        screenPoint.getZ() - getCamera().getO().getZ()
    );

    for(int i = 0; i < meshs.size(); i++) {
        for(int j = 0; j < meshs[i].getMesh().size(); j++) {
            tuple<int,int,int> vertices = meshs[i].getMesh().at(j);
            
            Vector normal = meshs[i].getNormals().at(j);
            
            Point v0 = meshs[i].getVertices().at(get<0>(vertices));
            Point v1 = meshs[i].getVertices().at(get<1>(vertices));
            Point v2 = meshs[i].getVertices().at(get<2>(vertices));

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
                    hits.insert({t, meshs[i].getColor(), meshs[i].getProps(), VectorOperations::normalize(normal), trianglePlanePoint});
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
            hits.insert({t, planes[i].getColor(), planes[i].getProps(), VectorOperations::normalize(planes[i].getNormal()), Point(getCamera().getO().getX() + (ray.getX() * t), getCamera().getO().getY() + (ray.getY() * t), getCamera().getO().getZ() + (ray.getZ() * t))});
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
            if(t1>0 && t2>0){
                Point intersection = Point(
                    getCamera().getO().getX() + (ray.getX() * min(t1, t2)),
                    getCamera().getO().getY() + (ray.getY() * min(t1, t2)),
                    getCamera().getO().getZ() + (ray.getZ() * min(t1, t2))
                );

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ()
                );

                hits.insert({min(t1, t2), spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});
            }else if(t1>0) {
                Point intersection = Point(
                    getCamera().getO().getX() + (ray.getX() * t1),
                    getCamera().getO().getY() + (ray.getY() * t1),
                    getCamera().getO().getZ() + (ray.getZ() * t1)
                );

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ()
                );

                hits.insert({t1, spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});

            }else if(t2>0) {
                Point intersection = Point(
                    getCamera().getO().getX() + (ray.getX() * t2),
                    getCamera().getO().getY() + (ray.getY() * t2),
                    getCamera().getO().getZ() + (ray.getZ() * t2)
                );

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ()
                );

                hits.insert({t2, spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});
            }
        }
    }
    return (hits.empty() ? RGB(0,0,0) : lighting(lights, make_tuple(get<1>(*hits.begin()), get<2>(*hits.begin()), get<3>(*hits.begin()), get<4>(*hits.begin())), RGB(0, 0, 0)));
}

RGB Scene::lighting(vector<Light> &lights, tuple<RGB, array<double, 6>, Vector, Point> &objInfo, RGB &ambientLight) {
    RGB light = RGB(get<0>(objInfo).r + ambientLight.r, get<0>(objInfo).g + ambientLight.g, get<0>(objInfo).b + ambientLight.b);
    Point intersection = get<3>(objInfo);
    Vector normal = get<2>(objInfo);
    
    for(int i = 0; i < lights.size(); i++) {
        double kd = get<1>(objInfo)[1];
        Point p = get<3>(objInfo);
        
        double coss = VectorOperations::dot(get<2>(objInfo), VectorOperations::normalize(
                Vector(
                    lights[i].getO().getX() - p.getX(),
                    lights[i].getO().getY() - p.getY(),
                    lights[i].getO().getZ() -p.getZ()
                )));
        //Difuse Light
        light = RGB(
            std::min(255, int(light.r + lights[i].getColor().r * kd * std::max(0.0, coss))),
            std::min(255, int(light.g + lights[i].getColor().g * kd * std::max(0.0, coss))),
            std::min(255, int(light.b + lights[i].getColor().b * kd * std::max(0.0, coss)))
        );

        Vector v = VectorOperations::normalize(Vector(
            getCamera().getO().getX() - intersection.getX(),
            getCamera().getO().getY() - intersection.getY(),
            getCamera().getO().getZ() - intersection.getZ()
        ));

        Vector l = VectorOperations::normalize(Vector(
            intersection.getX() - lights[i].getO().getX(),
            intersection.getY() - lights[i].getO().getY(),
            intersection.getZ() - lights[i].getO().getZ()
        ));

        Vector r = VectorOperations::normalize(Vector(VectorOperations::scale(Vector(
            normal.getX() - l.getX(),
            normal.getY() - l.getY(),
            normal.getZ() - l.getZ()
        ), (2 * coss))));

        double ks = get<1>(objInfo)[2];

        double angleFactor = max(0.0, VectorOperations::dot(r, v));

        light = RGB(
            std::min(255LL, long long int(light.r + lights[i].getColor().r * ks * std::max(pow(std::max(0.0, angleFactor), get<1>(objInfo)[3]), 0.0))),
            std::min(255LL, long long int(light.g + lights[i].getColor().g * ks * std::max(pow(std::max(0.0, angleFactor), get<1>(objInfo)[3]), 0.0))),
            std::min(255LL, long long int(light.b + lights[i].getColor().b * ks * std::max(pow(std::max(0.0, angleFactor), get<1>(objInfo)[3]), 0.0)))
        );
    }
    return light;
}