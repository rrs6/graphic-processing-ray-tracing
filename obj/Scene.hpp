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
    bool isShadow(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const Light &light, Point &point) const;
    void setCamera(const Camera& camera);
    void setWidth(int width);
    void setHeight(int height);
    void setScreenDistance(double distance);
    void render(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const vector<Light> &lights);
    RGB checkIntersections(const vector<Sphere> &spheres, const vector<Plan> &planes, const vector<TriangleMesh> &meshs, Vector &ray, Vector &refRay, const Point &rayO, const vector<Light> &lights, int depth);
    RGB lighting(const vector<Plan> &planes, const vector<Sphere> &spheres, const vector<TriangleMesh> &meshs, const vector<Light> &lights, const tuple<RGB, MaterialProperties, Vector, Point> &objInfo, const RGB &ambientLight);
};

#endif
