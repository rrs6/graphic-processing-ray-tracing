#include "../obj/TriangleMesh.hpp"

TriangleMesh::TriangleMesh(const std::vector<Point> &vertices, const std::vector<std::tuple<int, int, int>> &mesh, const std::vector<Vector> &normals, const RGB &color) : vertices(vertices), mesh(mesh), normals(normals), color(color) {}

std::vector<Point> TriangleMesh::getVertices() const { return vertices; }

std::vector<std::tuple<int, int, int>> TriangleMesh::getMesh() const { return mesh; }

std::vector<Vector> TriangleMesh::getNormals() const { return normals; }

RGB TriangleMesh::getColor() const {return color; }

void TriangleMesh::transform(const double (&m)[4][4]) {
    for(int v = 0; v < vertices.size(); v++) {
        vertices[v] = Point(
            (vertices[v].getX() * m[0][0]) + (vertices[v].getY() * m[0][1]) + (vertices[v].getZ() * m[0][2]) + m[0][3],
            (vertices[v].getX() * m[1][0]) + (vertices[v].getY() * m[1][1]) + (vertices[v].getZ() * m[1][2]) + m[1][3],
            (vertices[v].getX() * m[2][0]) + (vertices[v].getY() * m[2][1]) + (vertices[v].getZ() * m[2][2]) + m[2][3]
        );
    }
    for(int n = 0; n < normals.size(); n++){
        normals[n] = Vector(
            (normals[n].getX() * m[0][0]) + (normals[n].getY() * m[0][1]) + (normals[n].getZ() * m[0][2]),
            (normals[n].getX() * m[1][0]) + (normals[n].getY() * m[1][1]) + (normals[n].getZ() * m[1][2]),
            (normals[n].getX() * m[2][0]) + (normals[n].getY() * m[2][1]) + (normals[n].getZ() * m[2][2])
        );
    }
} 