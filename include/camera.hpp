#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


class Camera
{
public:
    Camera();
    Camera(glm::vec3 pos);

    void moveFront(float offset);
    void moveBackward(float offset);
    void moveLeft(float offset);
    void moveRight(float offset);
    void moveUp(float offset);
    void moveDown(float offset);
    void rotate(float xoffset, float yoffset);
    void zoom(float yoffset);

    void enable();
    void disable();

    glm::mat4 getViewMatrix();
    float getFOV();

    glm::vec3 getCoords();


private:
    glm::vec3 camera_pos = glm::vec3(0.0f);
    glm::vec3 camera_front;
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f; // camera points negative z-axis
    float pitch = 0.0f;
    float fov = 45.0f;

    bool camera_enabled = true;

    const float camera_speed = 5.0f;
};