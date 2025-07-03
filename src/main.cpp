#include <iostream>
#include "../core/Vector.hpp"
#include "../core/Point.hpp"
#include "../utils/VectorOperations.hpp"
#include "../obj/Scene.hpp"

int main() {
    Point origin(0.0, 0.0, 0);
    Point target(0.0, 0, 10);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 5;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 800;
    int altura = 600;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render();
    return 0;
}
