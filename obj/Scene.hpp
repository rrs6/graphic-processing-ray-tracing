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
    void render(const vector<TriangleMesh> &meshs, const vector<Light> &lights);
    RGB checkIntersections(const vector<Sphere> &spheres, const vector<Plan> &planes, const vector<TriangleMesh> &meshs, Point &screenPoint, const vector<Light> &lights);
    RGB lighting(const vector<Light> &lights, const tuple<RGB, MaterialProperties, Vector, Point> &objInfo, const RGB &ambientLight);
};

#endif
