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
        {1, 0, 0, 0},
        {0, 0.866, 0.988, 0},
        {0, -0.988, 0.866, 0},
        {0, 0, 0, 1}
    });
   
    Point origin(3.0, 0.0, -10);
    Point target(0.0, 0, 0);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 800;
    int altura = 600;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render(meshs, vector<Light>{Light(Point(-50,0,10), RGB(0,100,0)), Light(Point(-2,2,-2), RGB(0,0,150))});
    return 0;
}
