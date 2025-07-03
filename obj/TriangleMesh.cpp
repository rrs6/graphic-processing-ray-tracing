#include "../obj/TriangleMesh.hpp"

TriangleMesh::TriangleMesh(const std::vector<Point> &vertices, const std::vector<std::tuple<int, int, int>> &mesh, const std::vector<Vector> &normals, const RGB &color) : vertices(vertices), mesh(mesh), normals(normals), color(color) {}

std::vector<Point> TriangleMesh::getVertices() const { return vertices; }

std::vector<std::tuple<int, int, int>> TriangleMesh::getMesh() const { return mesh; }

std::vector<Vector> TriangleMesh::getNormals() const { return normals; }

RGB TriangleMesh::getColor() const {return color; }