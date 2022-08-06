#include <window/Camera.hpp>

#include <window/Window.hpp>

#include <glm/ext.hpp>

namespace window
{

Camera::Camera(const glm::vec3 &position, float fieldOfView) : position_(position), rotation_(1.0f)
{
    updateVectors();
}

void Camera::updateVectors()
{
    front_ = glm::vec3(rotation_ * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
    right_ = glm::vec3(rotation_ * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    up_ = glm::vec3(rotation_ * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void Camera::rotate(float x, float y, float z)
{
    rotation_ = glm::rotate(rotation_, x, glm::vec3(0.0f, 0.0f, 1.0f));
    rotation_ = glm::rotate(rotation_, y, glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_ = glm::rotate(rotation_, z, glm::vec3(1.0f, 0.0f, 0.0f));

    updateVectors();
}

glm::mat4 Camera::GetProjection()
{
    float aspect = Window::height_ ? Window::width_ / Window::height_ : 0;
    return glm::perspective(fov_, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::GetPosition()
{
    return position_;
}

glm::vec3 Camera::GetFront()
{
    return front_;
}

glm::vec3 Camera::GetUp()
{
    return up_;
}

glm::vec3 Camera::GetRight()
{
    return right_;
}

glm::mat4 Camera::GetRotation()
{
    return rotation_;
}

void Camera::SetPosition(glm::vec3 newPosition)
{
    position_ = newPosition;
}

void Camera::SetRotation(glm::mat4 newRotation)
{
    rotation_ = newRotation;
}

} // namespace window
