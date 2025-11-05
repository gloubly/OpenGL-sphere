#include "camera.hpp"
#include <iostream>
#include <glm/gtx/io.hpp>

Camera::Camera() {
    // init camera_front
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

Camera::Camera(glm::vec3 pos) {
    camera_pos = pos;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}

void Camera::moveFront(float offset) {
    if (!camera_enabled) return;
    glm::vec3 direction = glm::normalize(glm::vec3(camera_front.x, 0.0f, camera_front.z));
    camera_pos += camera_speed * offset * direction;
}

void Camera::moveBackward(float offset) {
    if (!camera_enabled) return;
    glm::vec3 direction = glm::normalize(glm::vec3(camera_front.x, 0.0f, camera_front.z));
    camera_pos -= camera_speed * offset * direction;
}

void Camera::moveLeft(float offset) {
    if (!camera_enabled) return;
    camera_pos -= camera_speed * offset * glm::normalize(glm::cross(camera_front, camera_up));
}

void Camera::moveRight(float offset) {
    if (!camera_enabled) return;
    camera_pos += camera_speed * offset * glm::normalize(glm::cross(camera_front, camera_up));
}

void Camera::moveUp(float offset) {
    if(!camera_enabled) return;
    camera_pos.y += offset * camera_speed;
}

void Camera::moveDown(float offset) {
    if (!camera_enabled) return;
    camera_pos.y -= offset * camera_speed;
}

void Camera::rotate(float xoffset, float yoffset) {
    if (!camera_enabled) return;
    yaw += xoffset;
    pitch += yoffset;
    if(pitch < -89.0f) {
        pitch = -89.0f;
    } else if(pitch > 89.0f) {
        pitch = 89.0f;
    }
    glm::vec3 direction = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );
    camera_front = glm::normalize(direction);
}

void Camera::zoom(float yoffset) {
    fov -= yoffset;
    if(fov < 1.0f) {
        fov = 1.0f;
    } else if(fov > 45.0f) {
        fov = 45.0f;
    }
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(
        camera_pos, camera_pos + camera_front, camera_up
    );
}

float Camera::getFOV() {
    return fov;
}

glm::vec3 Camera::getCoords() {
    return camera_pos;
}

void Camera::enable() {
    camera_enabled = true;
}

void Camera::disable() {
    camera_enabled = false;
}