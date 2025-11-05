#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cassert>

#ifndef M_PI
#define M_PI 3.141592
#endif // !M_PI

class Sphere {
public:
    Sphere(int nb_points, float radius);
    ~Sphere();

    void draw();

    void print_vertices();
    void print_indices();

private:
    float* vertices;
    unsigned int* indices;

    size_t vertices_length;
    size_t indices_length;

    unsigned int vao, vbo, ebo;
};