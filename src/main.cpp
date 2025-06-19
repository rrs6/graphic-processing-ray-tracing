#include <iostream>
#include "../core/Vector.hpp"
#include "../core/Point.hpp"
#include "../utils/VectorOperations.hpp"
#include "../obj/Scene.hpp"

int main() {
    Point origin(0.0, 0.0, 5);
    Point target(0.0, 0, 0);
    Vector up(0.0, 1.0, 0.0);

    double distanciaTela = 1;
    Camera camera(origin, target, up, distanciaTela);

    int largura = 500;
    int altura = 500;

    Scene scene(camera, largura, altura, camera.getD());

    scene.render();
    return 0;
}
