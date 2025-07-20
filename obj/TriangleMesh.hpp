#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include <vector>
#include <tuple>
#include "../core/Point.hpp"
#include "../utils/RGB.hpp"
#include "../core/Vector.hpp"
#include <array>
#include "Colormap.cpp"

using namespace std;
class TriangleMesh
{
private:
    vector<Point> vertices;
    vector<tuple<int, int, int>> mesh;
    vector<Vector> normals;
    RGB color;
    MaterialProperties props;

public:
    TriangleMesh(const vector<Point> &vertices, const vector<tuple<int, int, int>> &mesh, const vector<Vector> &normals, const RGB &color, const MaterialProperties &props);
    vector<Point> getVertices() const;
    vector<tuple<int, int, int>> getMesh() const;
    vector<Vector> getNormals() const;
    RGB getColor() const;
    void transform(const double (&m)[4][4]);
    MaterialProperties getProps() const;
};

#endif