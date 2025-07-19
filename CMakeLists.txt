cmake_minimum_required(VERSION 3.10)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(core utils obj)

set(SOURCES
    core/Point.cpp
    core/Vector.cpp
    utils/VectorOperations.cpp
    obj/Light.cpp
    obj/Camera.cpp
    obj/Scene.cpp
    obj/Sphere.cpp
    obj/Plan.cpp
    obj/TriangleMesh.cpp
    obj/Colormap.cpp
    obj/ObjReader.cpp
    src/main.cpp

)

add_executable(raytracing_app ${SOURCES})