#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "model.hpp"

Model::Model(const Mesh &mesh, const Shader &shader)
    : m_mesh(mesh), m_shader(shader)
{
    m_scale = glm::mat4{1.0f};
    m_rotation = glm::mat4{1.0f};
    m_translation = glm::mat4{1.0f};
}

const Mesh &Model::mesh() const
{
    return m_mesh;
}

const Shader &Model::shader() const
{
    return m_shader;
}

glm::mat4 Model::modelToWorld() const
{
    return m_translation * m_rotation * m_scale;
}

void Model::rotate(const glm::vec3 &axis, float deg)
{
    m_rotation = glm::rotate(m_rotation, deg, axis);
}

void Model::translate(const glm::vec3 &v)
{
    m_translation = glm::translate(m_translation, v);
}