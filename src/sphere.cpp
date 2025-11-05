#include "sphere.hpp"

Sphere::Sphere(int nb_points, float radius) {
    assert(("nb points must be odd", nb_points % 2 == 0));

    size_t nb_lat_rings = nb_points / 2 - 1;
    size_t num_vertices = (nb_points * nb_lat_rings + 2); // 2 for north and south poles
    vertices_length = num_vertices * 6; // coords (3) + normal vector (3)
    indices_length = nb_points * nb_lat_rings * 6;

    vertices = static_cast<float*>(malloc(vertices_length * sizeof(float)));
    indices = static_cast<unsigned int*>(malloc(indices_length * sizeof(unsigned int)));

    // theta : latitude [-pi ; pi]
    // phi : longitude [0, 2pi]
    const double delta_angle = 2 * M_PI / nb_points;
    double theta, phi;
    float x, y, z;
    size_t k = 0;
    for (size_t i = 1; i <= nb_lat_rings; ++i) {
        theta = -M_PI + i * delta_angle;
        for (size_t j = 0; j < nb_points; ++j) {
            phi = j * delta_angle;
            x = static_cast<float>(sin(theta) * cos(phi));
            y = static_cast<float>(cos(theta));
            z = static_cast<float>(sin(theta) * sin(phi));
            // coords
            vertices[k] = radius * x;
            vertices[k + 1] = radius * y;
            vertices[k + 2] = radius * z;
            // normal vector
            vertices[k + 3] = x;
            vertices[k + 4] = y;
            vertices[k + 5] = z;
            k += 6;
        }
    }

    // set north and south poles
    const unsigned int SOUTH_INDEX = vertices_length / 6 - 2;
    const unsigned int NORTH_INDEX = vertices_length / 6 - 1;
    for (size_t k = 1; k <= 12; ++k) {
        vertices[vertices_length - k] = 0;
    }
    vertices[vertices_length - 11] = -radius;
    vertices[vertices_length - 8] = -1;
    vertices[vertices_length - 5] = radius;
    vertices[vertices_length - 2] = 1;

    // bottom and top triangles
    size_t top_ring = (nb_lat_rings - 1) * (nb_points);
    for (size_t j = 0; j < nb_points; ++j) {
        indices[indices_length - j * 3 - 3] = top_ring + j;
        indices[indices_length - j * 3 - 2] = NORTH_INDEX;
        indices[indices_length - j * 3 - 1] = top_ring + (j + 1) % nb_points;

        indices[j * 3] = j;
        indices[j * 3 + 1] = SOUTH_INDEX;
        indices[j * 3 + 2] = (j + 1) % nb_points;
    }

    k = nb_points * 3;
    for (size_t i = 0; i < nb_lat_rings - 1; ++i) {
        for (size_t j = 0; j < nb_points; ++j) {

            // vertices index
            unsigned int v_left_bottom = i * nb_points + j;
            unsigned int v_right_bottom = i * nb_points + (j + 1) % nb_points;
            unsigned int v_left_top = (i + 1) * nb_points + j;
            unsigned int v_right_top = (i + 1) * nb_points + (j + 1) % nb_points;

            // triangles (counter clockwise)
            indices[k] = v_left_bottom;
            indices[k + 1] = v_right_bottom;
            indices[k + 2] = v_right_top;

            indices[k + 3] = v_left_bottom;
            indices[k + 4] = v_right_top;
            indices[k + 5] = v_left_top;

            k += 6;
        }
    }

    // setup the buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_length * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //print_vertices();
    //std::cout << std::endl;
    //print_indices();
}

Sphere::~Sphere() {
    free(vertices);
    vertices = NULL;

    free(indices);
    indices = NULL;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Sphere::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices_length, GL_UNSIGNED_INT, 0);
}

void Sphere::print_vertices() {
    std::cout << "sphere vertices:" << std::endl;
    for (size_t i = 0; i < vertices_length; i += 6) {
        std::cout << std::round(10 * vertices[i]) / 10.0 << " " << std::round(10 * vertices[i + 1]) / 10.0 << " " << std::round(10 * vertices[i + 2]) / 10.0 << " ";
        std::cout << std::round(10 * vertices[i+3]) / 10.0 << " " << std::round(10 * vertices[i + 4]) / 10.0 << " " << std::round(10 * vertices[i + 5]) / 10.0 << std::endl;
    }
}

void Sphere::print_indices() {
    std::cout << "sphere indices:" << std::endl;
    for (size_t i = 0; i < indices_length; i += 3) {
        std::cout << indices[i] << " " << indices[i + 1] << " " << indices[i + 2] << std::endl;
    }
}