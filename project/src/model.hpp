#ifndef MODEL_HPP
#define MODEL_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "mesh.hpp"
#include "shader.hpp"

class Model
{
    Mesh m_mesh;
    Shader m_shader;
    
    glm::mat4 m_translation;
    glm::mat4 m_scale;
    glm::mat4 m_rotation;

  public:
    Model(const Mesh &mesh, const Shader &shader);
    const Mesh &mesh() const;
    const Shader &shader() const;
    glm::mat4 modelToWorld() const;
    void rotate(const glm::vec3 &axis, GLfloat deg);
    void translate(const glm::vec3 &v);
    void scale(const glm::vec3 &s);
};

#endif