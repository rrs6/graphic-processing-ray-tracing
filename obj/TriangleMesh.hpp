#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include <vector>
#include <tuple>
#include "../core/Point.hpp"
#include "../utils/RGB.hpp"
#include "../core/Vector.hpp"

class TriangleMesh
{
private:
    std::vector<Point> vertices;
    std::vector<std::tuple<int, int, int>> mesh;
    std::vector<Vector> normals;
    RGB color;
public:
    TriangleMesh(const std::vector<Point> &vertices, const std::vector<std::tuple<int, int, int>> &mesh, const std::vector<Vector> &normals, const RGB &color);
    std::vector<Point> getVertices() const;
    std::vector<std::tuple<int, int, int>> getMesh() const;
    std::vector<Vector> getNormals() const;
    RGB getColor() const;
};

#endif