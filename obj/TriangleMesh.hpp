#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include <vector>
#include <tuple>
#include "../core/Point.hpp"
#include "../utils/RGB.hpp"
#include "../core/Vector.hpp"
#include <array>

using namespace std;
class TriangleMesh
{
private:
    vector<Point> vertices;
    vector<tuple<int, int, int>> mesh;
    vector<Vector> normals;
    RGB color;
    array<double, 6> props;

public:
    TriangleMesh(const vector<Point> &vertices, const vector<tuple<int, int, int>> &mesh, const vector<Vector> &normals, const RGB &color, const array<double, 6> &props);
    vector<Point> getVertices() const;
    vector<tuple<int, int, int>> getMesh() const;
    vector<Vector> getNormals() const;
    RGB getColor() const;
    void transform(const double (&m)[4][4]);
    array<double, 6> getProps() const;
};

#endif