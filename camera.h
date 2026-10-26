#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    glm::vec3 pos_{ 0.f, 1.6f, 8.f };
    glm::vec3 front_{ 0.f, 0.f,-1.f };
    glm::vec3 up_{ 0.f, 1.f, 0.f };
    float yaw_{ -90.f };
    float pitch_{ 0.f };

public:
    Camera() = default;

    glm::mat4 view() const;
    void key(char direction, float dt);
    void mouse(float dx, float dy);

   
    glm::vec3 getPosition() const { return pos_; }
};