#include <iostream>
#include "../core/Vector.hpp"
#include "../core/Point.hpp"
#include "../utils/VectorOperations.hpp"
#include "../obj/Scene.hpp"
#include "../obj/ObjReader.cpp"

int main() {
    objReader obj("../inputs/cubo.obj");
    vector<TriangleMesh> meshs = {obj.getMesh()};
   
    Point origin(3.0, 0.0, -10);
    Point target(0.0, 0, 0);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 800;
    int altura = 600;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render(meshs, vector<Light>{Light(Point(20,20,10), RGB(0,200, 200))});
    return 0;
}
