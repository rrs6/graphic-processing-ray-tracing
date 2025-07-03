#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "../core/Point.hpp"
#include "../core/Vector.hpp"
#include "../obj/Camera.hpp"
#include "../obj/Sphere.hpp"
#include "../obj/Plan.hpp"
#include "../utils/RGB.hpp"
#include "../obj/TriangleMesh.hpp"

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
    RGB checkIntersections(std::vector<Sphere> &spheres, std::vector<Plan> &planes, std::vector<TriangleMesh> &meshs, Point &screenPoint);
};

#endif
