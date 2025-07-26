#include <iostream>
#include "../core/Vector.hpp"
#include "../core/Point.hpp"
#include "../utils/VectorOperations.hpp"
#include "../obj/Scene.hpp"
#include "../obj/ObjReader.cpp"

int main() {
    objReader obj("../inputs/cubo.obj");
    vector<TriangleMesh> meshs = {obj.getMesh()};
    meshs[0].transform({
        {1, 0, 0, 2},
        {0, 1, 0, -5},
        {0, 0, 1, 10},
        {0, 0, 0, 1}
    });
    MaterialProperties m = MaterialProperties();
    m.ka = Vector(0, 0, 0);
    m.kd = Vector(1, 1, 1);
    m.ks = Vector(0, 0, 0);
    m.kr = Vector(1, 1, 1);
    m.kt = Vector(0, 0, 0);
    m.d = 1;
    vector<Sphere> spheres = {Sphere(Point(-6, 0, 10), 2, RGB(50, 0, 0), m), Sphere(Point(1, 0, 15), 2, RGB(0, 0, 100), m)};
    vector<Plan> planes = {Plan(Point(0, 0, 40), Vector(-3, 0, -1), RGB(0, 0, 50), m)};

    //Sphere(Point(4, 0, 20), 1, RGB(50, 50, 50), m)

    Point origin(0, 0.0,-10);
    Point target(0.0, 0, 10);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 800;
    int altura = 600;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render({}, spheres , {}, vector<Light>{Light(Point(0, 0, 0), RGB(0,60,0))});
    return 0;
}
