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
        {1, 0, 0, 1},
        {0, 1, 0, -1},
        {0, 0, 1, 4},
        {0, 0, 0, 1}
    });

    MaterialProperties m = MaterialProperties();
    m.ka = Vector(0, 0, 0);
    m.kd = Vector(1, 1, 1);
    m.ks = Vector(1, 1, 1);
    m.kr = Vector(1, 1, 1);
    m.kt = Vector(1, 1, 1);
    m.d = 1.2;

    MaterialProperties m1 = MaterialProperties();
    m1.ka = Vector(0, 0, 0);
    m1.kd = Vector(1, 1, 1);
    m1.ks = Vector(1, 1, 1);
    m1.kr = Vector(0, 0, 0);
    m1.kt = Vector(0, 0, 0);
    m1.d = 0;


    vector<Sphere> spheres = {Sphere(Point(-1, 0, 6), 1, RGB(50, 0, 0), m1)};
    vector<Plan> planes = {Plan(Point(0, 0, 3), Vector(0, 0, -1), RGB(0, 0, 50), m)};

    //Sphere(Point(4, 0, 20), 1, RGB(50, 50, 50), m)

    Point origin(0, 0.0,-10);
    Point target(0.0, 0, 10);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    // camera.transform({
    // {1,     0,      0,     0},
    // {0,  0.9848, -0.1736,  0},
    // {0,  0.1736,  0.9848,  0},
    // {0,     0,      0,     1}
    // });

//    camera.transform({
//     { 0.8660, 0,  0.5,    0},
//     { 0,      1,  0,      0},
//     {-0.5,    0,  0.8660, 0},
//     { 0,      0,  0,      1}
// });


    int largura = 500;
    int altura = 500;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render(planes, spheres , {}, {vector<Light>{Light(Point(0, 0, -2), RGB(30,30,30))}});
    return 0;
}
