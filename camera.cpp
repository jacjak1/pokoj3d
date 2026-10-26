#include "camera.h"


glm::mat4 Camera::view() const
{
    return glm::lookAt(pos_, pos_ + front_, up_);
}


void Camera::key(char d, float dt)
{
    const float v = 2.5f * dt;                 
    if (d == 'W') pos_ += front_ * v;
    if (d == 'S') pos_ -= front_ * v;
    if (d == 'A') pos_ -= glm::normalize(glm::cross(front_, up_)) * v;
    if (d == 'D') pos_ += glm::normalize(glm::cross(front_, up_)) * v;
}

void Camera::mouse(float dx, float dy)
{
    const float sens = 0.1f;
    yaw_ += dx * sens;
    pitch_ -= dy * sens;
    pitch_ = glm::clamp(pitch_, -89.f, 89.f);

    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    dir.y = sin(glm::radians(pitch_));
    dir.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(dir);
}
