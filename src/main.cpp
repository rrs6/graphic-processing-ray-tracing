#include <iostream>
#include "../core/Vector.hpp"
#include "../core/Point.hpp"
#include "../utils/VectorOperations.hpp"
#include "../obj/Scene.hpp"
#include "../obj/ObjReader.cpp"

int main() {
    objReader obj("../inputs/cubo.obj");
    vector<TriangleMesh> meshs = {obj.getMesh()};
    // meshs[0].transform({
    //     {1, 0, 0, 0},
    //     {0, 0.866, 0.988, 0},
    //     {0, -0.988, 0.866, 0},
    //     {0, 0, 0, 1}
    // });
    MaterialProperties m = MaterialProperties();
    m.ka = Vector(1, 1, 1);
    m.kd = Vector(1, 1, 1);
    m.ks = Vector(0.6, 0.6, 0.6);
    vector<Sphere> spheres = {Sphere(Point(-1, 0, 12), 2, RGB(50, 50, 50), m), Sphere(Point(4, 0, 30), 1, RGB(50, 50, 50), m)};
   
    Point origin(0, 0.0,-10);
    Point target(0.0, 0, 10);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 800;
    int altura = 600;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render({}, spheres , {}, vector<Light>{Light(Point(-3, 3, -1), RGB(0,90,0)), Light(Point(20, 0, 12), RGB(90,0,0))});
    return 0;
}
