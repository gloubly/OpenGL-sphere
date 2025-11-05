#include "shader.hpp"
#include "stb_image.h"
#include "camera.hpp"
#include "sphere.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <fstream>
#include <memory>

unsigned int SCR_WIDTH = 900;
unsigned int SCR_HEIGHT = 900;

const float mouse_sensitivity = 0.03f;

auto camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f)); // global for now but should be placed in a window class

float last_mouse_x = 0.0f;
float last_mouse_y = 0.0f;

float delta_time = 0.0f;
float last_frame = 0.0f;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
    glViewport(0, 0, width, height);
}

void mouseMotionCallback(GLFWwindow* window, double xpos, double ypos) {
    float xpos_f = static_cast<float>(xpos);
    float ypos_f = static_cast<float>(ypos);
    float xoffset = (xpos_f - last_mouse_x) * mouse_sensitivity;
    float yoffset = (last_mouse_y - ypos_f) * mouse_sensitivity;
    last_mouse_x = xpos_f;
    last_mouse_y = ypos_f;
    camera.rotate(xoffset, yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) return; // only handle press events
    if (key == GLFW_KEY_1) {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera.disable();
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.enable();
        }
    }
}

void processInput(GLFWwindow* window, Camera& camera) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveFront(delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft(delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward(delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.moveUp(delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.moveDown(delta_time);
    }
}

int main() {
    stbi_set_flip_vertically_on_load(true);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetCursorPosCallback(window, mouseMotionCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    auto sphere = std::make_unique<Sphere>(50, 2.0f);

    Shader light_source_shader("resources/shaders/3d.vert", "resources/shaders/light_source.frag");
    Shader light_shader("resources/shaders/3d.vert", "resources/shaders/lighting.frag");


    float cube_vertices[] = {
        // position            normal
        -0.5f, -0.5f,  0.5f,   -0.5f, -0.5f,  0.5f, // bottom front left
         0.5f, -0.5f,  0.5f,    0.5f, -0.5f,  0.5f, // bottom front right
        -0.5f, -0.5f, -0.5f,   -0.5f, -0.5f, -0.5f, // bottom back left
         0.5f, -0.5f, -0.5f,    0.5f, -0.5f, -0.5f, // bottom back right

        -0.5f,  0.5f,  0.5f,   -0.5f,  0.5f,  0.5f, // top front left
         0.5f,  0.5f,  0.5f,    0.5f,  0.5f,  0.5f, // top front right
        -0.5f,  0.5f, -0.5f,   -0.5f,  0.5f, -0.5f, // top back left
         0.5f,  0.5f, -0.5f,    0.5f,  0.5f, -0.5f, // top back right
    };

    unsigned int cube_indices[] = {
        //bottom
        0, 1, 2,
        1, 2, 3,
        // front
        0, 4, 5,
        0, 1, 5,
        // back
        2, 3, 6,
        3, 6, 7,
        //right
        1, 3, 5,
        3, 5, 7,
        // left
        0, 2, 4,
        2, 4, 6,
        // top
        4, 5, 6,
        5, 6, 7
    };

    unsigned int cube_vao, cube_vbo, cube_ebo;

    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &cube_vbo);
    glGenBuffers(1, &cube_ebo);

    glBindVertexArray(cube_vao);

    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float time;
    float scale = 1.1f;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 light_pos = glm::vec3(0.0f, 4.0f, 3.0f);

    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        time = static_cast<float>(glfwGetTime());
        delta_time = time - last_frame;
        last_frame = time;

        processInput(window, camera);

        //glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 0.5 + sin(time)/2);

        view = camera.getViewMatrix();

        glm::mat4 projection = glm::perspective(
            camera.getFOV(), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 100.0f
        );

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        light_source_shader.use();
        light_source_shader.setMat4("view", view);
        light_source_shader.setMat4("projection", projection);
        light_source_shader.setMat4("model", model);
        light_source_shader.setVec3("color", light_color);
        glBindVertexArray(cube_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f));
        light_shader.use();
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("model", model);
        light_shader.setVec3("object_color", 0.4f, 0.1f, 0.6f);
        light_shader.setVec3("light_color", light_color);
        light_shader.setVec3("light_pos", light_pos);
        light_shader.setVec3("view_pos", camera.getCoords());
        sphere->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}