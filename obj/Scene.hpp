#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../obj/Camera.hpp"
#include "../obj/Sphere.hpp"
#include "../obj/Plan.hpp"
#include "../utils/RGB.hpp"
#include "Light.hpp"
#include "../obj/TriangleMesh.hpp"

using namespace std;

class Scene {
private:
    Camera camera;
    int width;
    int height;
    double screenDistance;

public:
    Scene(const Camera& camera, int width, int height, double distance);

    Camera getCamera() const;
    int getWidth() const;
    int getHeight() const;
    double getScreenDistance() const;

    void setCamera(const Camera& camera);
    void setWidth(int width);
    void setHeight(int height);
    void setScreenDistance(double distance);
    void render();
    RGB checkIntersections(vector<Sphere> &spheres, vector<Plan> &planes, vector<TriangleMesh> &meshs, Point &screenPoint, vector<Light> &lights);
    RGB lighting(vector<Light> &lights, tuple<RGB, array<double, 6>, Vector, Point> &objInfo, RGB &ambientLight);
};

#endif
