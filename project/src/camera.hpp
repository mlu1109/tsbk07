#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <iostream>

class Camera
{
    glm::vec3 m_position;
    glm::vec3 m_look;
    glm::vec3 m_up;

    float m_fovy = M_PI / 4.0f;
    float m_aspect = 4.0f/3.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;

    glm::mat4 m_viewToScreen;

  public:
    Camera(const glm::vec3 &position, const glm::vec3 &look, const glm::vec3 &up);
    void position(const glm::vec3 &position);
    void look(const glm::vec3 &look);
    void up(const glm::vec3 &up);

    void yaw(float rad);
    void pitch(float rad);
    void roll(float rad);
    void moveX(float amount);
    void moveZ(float amount);

    glm::mat4 worldToView() const;
    glm::mat4 worldToViewToScreen() const;

    void updateAspect(int windowWidth, int windowHeight); 
    
    friend std::ostream &operator<<(std::ostream &os, const Camera &c);
};

#endif