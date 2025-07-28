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
#include "Colormap.cpp"

using namespace std;

Scene::Scene(const Camera &camera, int width, int height, double distance)
    : camera(camera), width(width), height(height), screenDistance(distance) {}

Camera Scene::getCamera() const { return camera; }

int Scene::getWidth() const { return width; }

int Scene::getHeight() const { return height; }

double Scene::getScreenDistance() const { return screenDistance; }

void Scene::setCamera(const Camera &camera) { this->camera = camera; }

void Scene::setWidth(int width) { this->width = width; }

void Scene::setHeight(int height) { this->height = height; }

void Scene::setScreenDistance(double distance) { screenDistance = distance; }

void Scene::render(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const vector<Light> &lights)
{

    Camera cam = getCamera();
    Vector front = cam.getFront();
    Vector up = cam.getUp();
    Vector right = cam.getW();

    Point screenO = Point(
        cam.getO().getX() + (front.getX() * screenDistance),
        cam.getO().getY() + (front.getY() * screenDistance),
        cam.getO().getZ() + (front.getZ() * screenDistance));
    double aspectRatio = static_cast<double>(width) / height;
    vector<vector<RGB>> image(height, vector<RGB>(width, RGB(255, 255, 255)));

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            double u = ((static_cast<double>(i) / (width - 1)) - 0.5) * 2 * aspectRatio;
            double v = ((static_cast<double>(j) / (height - 1)) - 0.5) * -2;

            Vector offset = VectorOperations::scale(right, u);
            offset = VectorOperations::sum(offset, VectorOperations::scale(up, v));

            Point screenPoint = Point(
                screenO.getX() + offset.getX(),
                screenO.getY() + offset.getY(),
                screenO.getZ() + offset.getZ());

            Vector ray = Vector(
                screenPoint.getX() - getCamera().getO().getX(),
                screenPoint.getY() - getCamera().getO().getY(),
                screenPoint.getZ() - getCamera().getO().getZ());

            image[j][i] = checkIntersections(spheres, planes,
                                             meshs, ray, getCamera().getO(), lights, 3);
        }
    }
    ofstream out("output.ppm");
    out << "P3\n"
        << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            RGB &pixel = image[j][i];
            out << int(pixel.r) << ' ' << int(pixel.g) << ' ' << int(pixel.b) << '\n';
        }
    }
    out.close();
}
RGB Scene::checkIntersections(const vector<Sphere> &spheres, const vector<Plan> &planes, const vector<TriangleMesh> &meshs, Vector &ray, const Point &rayO, const vector<Light> &lights, int depth)
{
    if (depth <= 0)
        return RGB(0, 0, 0);

    set<tuple<double, RGB, MaterialProperties, Vector, Point>, bool (*)(const tuple<double, RGB, MaterialProperties, Vector, Point> &, const tuple<double, RGB, MaterialProperties, Vector, Point> &)> hits(
        [](const tuple<double, RGB, MaterialProperties, Vector, Point> &a, const tuple<double, RGB, MaterialProperties, Vector, Point> &b)
        {
            return get<0>(a) < get<0>(b);
        });

    for (int i = 0; i < meshs.size(); i++)
    {
        for (int j = 0; j < meshs[i].getMesh().size(); j++)
        {
            tuple<int, int, int> vertices = meshs[i].getMesh().at(j);

            Vector normal = meshs[i].getNormals().at(j);

            Point v0 = meshs[i].getVertices().at(get<0>(vertices));
            Point v1 = meshs[i].getVertices().at(get<1>(vertices));
            Point v2 = meshs[i].getVertices().at(get<2>(vertices));

            double t = VectorOperations::dot(normal, Vector(
                                                         v1.getX() - rayO.getX(),
                                                         v1.getY() - rayO.getY(),
                                                         v1.getZ() - rayO.getZ())) /
                       VectorOperations::dot(normal, ray);

            if (t > 0)
            {
                Point trianglePlanePoint = Point(
                    rayO.getX() + VectorOperations::scale(ray, t).getX(),
                    rayO.getY() + VectorOperations::scale(ray, t).getY(),
                    rayO.getZ() + VectorOperations::scale(ray, t).getZ());

                Vector v0v1 = Vector(v1.getX() - v0.getX(), v1.getY() - v0.getY(), v1.getZ() - v0.getZ());
                Vector v0v2 = Vector(v2.getX() - v0.getX(), v2.getY() - v0.getY(), v2.getZ() - v0.getZ());
                Vector v0p = Vector(trianglePlanePoint.getX() - v0.getX(), trianglePlanePoint.getY() - v0.getY(), trianglePlanePoint.getZ() - v0.getZ());

                double d00 = VectorOperations::dot(v0v1, v0v1);
                double d01 = VectorOperations::dot(v0v1, v0v2);
                double d11 = VectorOperations::dot(v0v2, v0v2);
                double d20 = VectorOperations::dot(v0p, v0v1);
                double d21 = VectorOperations::dot(v0p, v0v2);

                double denom = d00 * d11 - d01 * d01;
                double v = (d11 * d20 - d01 * d21) / denom;
                double w = (d00 * d21 - d01 * d20) / denom;
                double u = 1.0 - v - w;

                if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                {
                    hits.insert({t, meshs[i].getColor(), meshs[i].getProps(), VectorOperations::normalize(normal), trianglePlanePoint});
                }
            }
        }
    }

    for (int i = 0; i < planes.size(); i++)
    {
        double t = VectorOperations::dot(planes[i].getNormal(),
                                         Vector(
                                             planes[i].getP().getX() - rayO.getX(),
                                             planes[i].getP().getY() - rayO.getY(),
                                             planes[i].getP().getZ() - rayO.getZ())) /
                   VectorOperations::dot(planes[i].getNormal(), ray);

        if (t > 0)
        {
            hits.insert({t, planes[i].getColor(), planes[i].getProps(), VectorOperations::normalize(planes[i].getNormal()), Point(rayO.getX() + (ray.getX() * t), rayO.getY() + (ray.getY() * t), rayO.getZ() + (ray.getZ() * t))});
        }
    }

    for (int i = 0; i < spheres.size(); i++)
    {

        Vector oc = Vector(
            rayO.getX() - spheres[i].getO().getX(),
            rayO.getY() - spheres[i].getO().getY(),
            rayO.getZ() - spheres[i].getO().getZ());

        double a = VectorOperations::dot(ray, ray);
        double b = 2.0 * VectorOperations::dot(oc, ray);
        double c = VectorOperations::dot(oc, oc) - (spheres[i].getR() * spheres[i].getR());

        const double delta = (b * b) - (4 * a * c);
        if (delta >= 0)
        {
            const double t1 = (-b + sqrt(delta)) / (2 * a);
            const double t2 = (-b - sqrt(delta)) / (2 * a);
            if (t1 > 0 && t2 > 0)
            {
                Point intersection = Point(
                    rayO.getX() + (ray.getX() * min(t1, t2)),
                    rayO.getY() + (ray.getY() * min(t1, t2)),
                    rayO.getZ() + (ray.getZ() * min(t1, t2)));

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ());

                hits.insert({min(t1, t2), spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});
            }
            else if (t1 > 0)
            {
                Point intersection = Point(
                    rayO.getX() + (ray.getX() * t1),
                    rayO.getY() + (ray.getY() * t1),
                    rayO.getZ() + (ray.getZ() * t1));

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ());

                hits.insert({t1, spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});
            }
            else if (t2 > 0)
            {
                Point intersection = Point(
                    rayO.getX() + (ray.getX() * t2),
                    rayO.getY() + (ray.getY() * t2),
                    rayO.getZ() + (ray.getZ() * t2));

                Vector normal = Vector(
                    intersection.getX() - spheres[i].getO().getX(),
                    intersection.getY() - spheres[i].getO().getY(),
                    intersection.getZ() - spheres[i].getO().getZ());

                hits.insert({t2, spheres[i].getColor(), spheres[i].getProps(), VectorOperations::normalize(normal), intersection});
            }
        }
    }
    RGB color = (hits.empty() ? RGB(0, 0, 0) : get<1>(*hits.begin()));
    MaterialProperties mp = (hits.empty() ? MaterialProperties() : get<2>(*hits.begin()));
    Vector nmrl = (hits.empty() ? Vector(0, 0, 0) : get<3>(*hits.begin()));
    Point intersection = (hits.empty() ? Point(0, 0, 0) : get<4>(*hits.begin()));

    RGB reflactionComponent(0, 0, 0);
    RGB refractionComponent(0, 0, 0);

    double dotIN = VectorOperations::dot(ray, nmrl);
    Vector reflectedRay = VectorOperations::normalize(
        VectorOperations::sum(ray, VectorOperations::dirChange(VectorOperations::scale(nmrl, 2 * dotIN))));

    Point intersect = Point(intersection.getX() + reflectedRay.getX() * 0.001, intersection.getY() + reflectedRay.getY() * 0.001, intersection.getZ() + reflectedRay.getZ() * 0.001);

    RGB reflect = checkIntersections(spheres, planes, meshs, reflectedRay, intersect, lights, depth - 1);
    reflactionComponent = RGB(min(255.0, reflect.r * mp.kr.getX()), min(255.0, reflect.g * mp.kr.getY()), min(255.0, reflect.b * mp.kr.getZ()));

    // double cosTeta = -VectorOperations::dot(VectorOperations::dirChange(ray), nmrl);
    // double cosTeta0 = sqrt((1 - (mp.d * mp.d)) * (1 - (cosTeta * cosTeta)));
    // Vector refractedRay = VectorOperations::sum(
    //     VectorOperations::scale(ray, 1/mp.d), (VectorOperations::scale(nmrl, mp.d*cosTeta - cosTeta0)));

    // Vector refractBias = VectorOperations::scale(refractedRay, 0.001);
    //     intersect = Point(
    //         intersection.getX() + refractBias.getX(),
    //         intersection.getY() + refractBias.getY(),
    //         intersection.getZ() + refractBias.getZ()
    //     );

    // RGB refract = checkIntersections(spheres, planes, meshs, refractedRay, intersect, lights, depth - 1);
    // refractionComponent = RGB(min(255.0, refractionComponent.r + refract.r * mp.kt.getX()), min(255.0, refractionComponent.g + refract.g * mp.kt.getY()), min(255.0, refractionComponent.b + refract.b * mp.kt.getZ()));

    Vector I = VectorOperations::normalize(ray); // direção do raio incidente
    Vector N = VectorOperations::normalize(nmrl);                             // normal da superfície no ponto de interseção

    // Detecta se o raio está vindo de dentro do objeto
    bool isInside = VectorOperations::dot(I, N) > 0;
    if (isInside) {
        N = VectorOperations::dirChange(N); // inverte normal se estiver dentro
    }

    // Índices de refração
    double n1 = isInside ? mp.d : 1.0; // de onde o raio está vindo
    double n2 = isInside ? 1.0 : mp.d; // para onde o raio vai
    double eta = 1 / mp.d;

    // Cálculo do cosseno do ângulo de incidência
    double cosI = -VectorOperations::dot(I, N);
    double sin2T = eta * eta * (1.0 - cosI * cosI);

    // Verifica reflexão total interna
    if (sin2T > 1.0) {
        // Reflexão total interna: não há refração
        refractionComponent = RGB(0, 0, 0);
    } else {
        // Cálculo do cosseno do ângulo transmitido
        double cosT = sqrt(1.0 - sin2T);

        // Calcula vetor refratado: T = ηI + (ηcosI - cosT)N
        Vector refractedRay = VectorOperations::normalize(
            VectorOperations::sum(
                VectorOperations::scale(I, eta),
                VectorOperations::scale(N, eta * cosI - cosT)));

        // Evita acne com pequeno deslocamento
        Vector refractBias = VectorOperations::scale(refractedRay, 0.001);
        Point refractOrigin = Point(
            intersection.getX() + refractBias.getX(),
            intersection.getY() + refractBias.getY(),
            intersection.getZ() + refractBias.getZ());

        // Lança o raio de refração recursivamente
        RGB refract = checkIntersections(
            spheres, planes, meshs, refractedRay, refractOrigin, lights, depth - 1);

        // Aplica coeficiente de transparência (kt)
        refractionComponent = RGB(
            min(255.0, refract.r * mp.kt.getX()),
            min(255.0, refract.g * mp.kt.getY()),
            min(255.0, refract.b * mp.kt.getZ()));
    }

    RGB actualLight = lighting(planes, spheres, meshs, lights, make_tuple(color, mp, nmrl, intersection), RGB(0, 0, 0));
    return RGB(min(255, reflactionComponent.r + refractionComponent.r + actualLight.r),
               min(255, reflactionComponent.g + refractionComponent.g + actualLight.g),
               min(255, reflactionComponent.b + refractionComponent.b + actualLight.b));
}

bool Scene::isShadow(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const Light &light, Point &p) const
{
    const double epsilon = 1e-4;

    Vector shadowRay = Vector(
        light.getO().getX() - p.getX(),
        light.getO().getY() - p.getY(),
        light.getO().getZ() - p.getZ());
    double maxDistance = sqrt(VectorOperations::dot(shadowRay, shadowRay));
    shadowRay = VectorOperations::normalize(shadowRay);

    Point origin = Point(
        p.getX() + shadowRay.getX() * epsilon,
        p.getY() + shadowRay.getY() * epsilon,
        p.getZ() + shadowRay.getZ() * epsilon);

    for (const auto &sphere : spheres)
    {
        Vector oc = Vector(
            origin.getX() - sphere.getO().getX(),
            origin.getY() - sphere.getO().getY(),
            origin.getZ() - sphere.getO().getZ());

        double a = VectorOperations::dot(shadowRay, shadowRay);
        double b = 2.0 * VectorOperations::dot(oc, shadowRay);
        double c = VectorOperations::dot(oc, oc) - sphere.getR() * sphere.getR();

        double delta = b * b - 4 * a * c;
        if (delta >= 0)
        {
            double t1 = (-b - sqrt(delta)) / (2 * a);
            double t2 = (-b + sqrt(delta)) / (2 * a);
            if ((t1 > 0 && t1 < maxDistance) || (t2 > 0 && t2 < maxDistance))
            {
                return true;
            }
        }
    }
    shadowRay = Vector(
        light.getO().getX() - p.getX(),
        light.getO().getY() - p.getY(),
        light.getO().getZ() - p.getZ());
    maxDistance = sqrt(VectorOperations::dot(shadowRay, shadowRay));
    shadowRay = VectorOperations::normalize(shadowRay);

    origin = Point(
        p.getX() + shadowRay.getX() * epsilon,
        p.getY() + shadowRay.getY() * epsilon,
        p.getZ() + shadowRay.getZ() * epsilon);

    for (int i = 0; i < planes.size(); i++)
    {
        double t = VectorOperations::dot(planes[i].getNormal(),
                                         Vector(planes[i].getP().getX() - origin.getX(),
                                                planes[i].getP().getY() - origin.getY(),
                                                planes[i].getP().getZ() - origin.getZ())) /
                   VectorOperations::dot(planes[i].getNormal(), shadowRay);

        if (t > 0 && t < maxDistance)
        {
            return true;
        }
    }

    for (int i = 0; i < meshs.size(); i++)
    {
        for (int j = 0; j < meshs[i].getMesh().size(); j++)
        {
            tuple<int, int, int> vertices = meshs[i].getMesh().at(j);
            Vector normal = meshs[i].getNormals().at(j);

            Point v0 = meshs[i].getVertices().at(get<0>(vertices));
            Point v1 = meshs[i].getVertices().at(get<1>(vertices));
            Point v2 = meshs[i].getVertices().at(get<2>(vertices));

            double t = VectorOperations::dot(normal, Vector(
                                                         v1.getX() - origin.getX(),
                                                         v1.getY() - origin.getY(),
                                                         v1.getZ() - origin.getZ())) /
                       VectorOperations::dot(normal, shadowRay);

            if (t > 0 && t < maxDistance)
            {
                Point trianglePlanePoint = Point(
                    origin.getX() + shadowRay.getX() * t,
                    origin.getY() + shadowRay.getY() * t,
                    origin.getZ() + shadowRay.getZ() * t);

                Vector v0v1 = Vector(v1.getX() - v0.getX(), v1.getY() - v0.getY(), v1.getZ() - v0.getZ());
                Vector v0v2 = Vector(v2.getX() - v0.getX(), v2.getY() - v0.getY(), v2.getZ() - v0.getZ());
                Vector v0p = Vector(trianglePlanePoint.getX() - v0.getX(), trianglePlanePoint.getY() - v0.getY(), trianglePlanePoint.getZ() - v0.getZ());

                double d00 = VectorOperations::dot(v0v1, v0v1);
                double d01 = VectorOperations::dot(v0v1, v0v2);
                double d11 = VectorOperations::dot(v0v2, v0v2);
                double d20 = VectorOperations::dot(v0p, v0v1);
                double d21 = VectorOperations::dot(v0p, v0v2);

                double denom = d00 * d11 - d01 * d01;
                double v = (d11 * d20 - d01 * d21) / denom;
                double w = (d00 * d21 - d01 * d20) / denom;
                double u = 1.0 - v - w;

                if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

RGB Scene::lighting(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const vector<Light> &lights, const tuple<RGB, MaterialProperties, Vector, Point> &objInfo, const RGB &ambientLight)
{
    RGB light = RGB(
        get<0>(objInfo).r + int(ambientLight.r * get<1>(objInfo).ka.getX()),
        get<0>(objInfo).g + int(ambientLight.g * get<1>(objInfo).ka.getY()),
        get<0>(objInfo).b + int(ambientLight.b * get<1>(objInfo).ka.getZ()));

    Point intersection = get<3>(objInfo);
    Vector normal = get<2>(objInfo);

    for (int i = 0; i < lights.size(); i++)
    {
        if (!isShadow(planes, spheres, meshs, lights[i], intersection))
        {
            Vector kd = get<1>(objInfo).kd;
            Point p = get<3>(objInfo);

            double coss = VectorOperations::dot(get<2>(objInfo), VectorOperations::normalize(
                                                                     Vector(
                                                                         lights[i].getO().getX() - p.getX(),
                                                                         lights[i].getO().getY() - p.getY(),
                                                                         lights[i].getO().getZ() - p.getZ())));
            // Difuse Light
            light = RGB(
                std::min(255LL, static_cast<long long int>(light.r + lights[i].getColor().r * kd.getX() * std::max(0.0, coss))),
                std::min(255LL, static_cast<long long int>(light.g + lights[i].getColor().g * kd.getY() * std::max(0.0, coss))),
                std::min(255LL, static_cast<long long int>(light.b + lights[i].getColor().b * kd.getZ() * std::max(0.0, coss))));

            Vector v = VectorOperations::normalize(Vector(
                getCamera().getO().getX() - intersection.getX(),
                getCamera().getO().getY() - intersection.getY(),
                getCamera().getO().getZ() - intersection.getZ()));

            Vector l = VectorOperations::normalize(Vector(
                intersection.getX() - lights[i].getO().getX(),
                intersection.getY() - lights[i].getO().getY(),
                intersection.getZ() - lights[i].getO().getZ()));

            Vector r = VectorOperations::normalize(Vector(VectorOperations::scale(Vector(
                                                                                      normal.getX() - l.getX(),
                                                                                      normal.getY() - l.getY(),
                                                                                      normal.getZ() - l.getZ()),
                                                                                  (2 * coss))));

            Vector ks = get<1>(objInfo).ks;

            double angleFactor = max(0.0, VectorOperations::dot(r, v));

            light = RGB(
                std::min(255LL, static_cast<long long int>(light.r + lights[i].getColor().r * ks.getX() * std::max(pow(std::max(0.0, angleFactor), 10.0), 0.0))),
                std::min(255LL, static_cast<long long int>(light.g + lights[i].getColor().g * ks.getY() * std::max(pow(std::max(0.0, angleFactor), 10.0), 0.0))),
                std::min(255LL, static_cast<long long int>(light.b + lights[i].getColor().b * ks.getZ() * std::max(pow(std::max(0.0, angleFactor), 10.0), 0.0))));
        }
    }
    return light;
}