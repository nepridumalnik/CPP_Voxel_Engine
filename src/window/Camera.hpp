#pragma once

namespace window
{

class EXPORT Camera
{
public:
    Camera(const glm::vec3 &position, float fieldOfView);

    glm::mat4 GetProjection();
    glm::mat4 GetView();

    glm::vec3 GetPosition();
    glm::vec3 GetFront();
    glm::vec3 GetUp();
    glm::vec3 GetRight();
    glm::mat4 GetRotation();

    void SetPosition(glm::vec3 newPosition);
    void SetRotation(glm::mat4 newRotation);
    void Rotate(float x, float y, float z);

private:
    void updateVectors();

private:
    float fov_;
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::mat4 rotation_;
};

} // namespace window
