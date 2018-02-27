#ifndef RENDER_HPP
#define RENDER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "model.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Renderer
{
    GLFWwindow *m_window;
    Camera *m_camera;
    std::unordered_map<ShaderType, Shader> m_shaders;
    glm::vec4 m_clearColor{0.2f, 0.2f, 0.5f, 1.0f};

    void drawMesh(GLuint program, const Mesh &mesh) const;
    void drawModel(const Model &m) const;
    void uploadMatrices(GLuint program, const glm::mat4 &mvp, const glm::mat3 &nmvp) const;

  public:
    Renderer(GLFWwindow *window, Camera *camera, const std::unordered_map<ShaderType, Shader> &shader);
    void render(const std::vector<Model> &models);
};

#endif