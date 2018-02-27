#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(GLFWwindow *window, Camera *camera, const std::unordered_map<ShaderType, Shader> &shaders)
    : m_window(window), m_camera(camera), m_shaders(shaders)
{
    glClearColor(0.2f, 0.2f, 0.5f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::uploadMatrices(GLuint program, const glm::mat4 &mvp, const glm::mat3 &nmvp) const
{
    /* MVP */
    GLint mvpLocation = glGetUniformLocation(program, "mvp");
    if (mvpLocation < 0)
        throw std::runtime_error("\"mvp\" was not found in shader");

    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

    /* MNVP */
    GLint nmvpLocation = glGetUniformLocation(program, "nmvp");
    if (nmvpLocation < 0)
        throw std::runtime_error("\"nmvp\" was not found in shader");

    glUniformMatrix3fv(nmvpLocation, 1, GL_FALSE, &nmvp[0][0]);
}

void Renderer::drawMesh(GLuint program, const Mesh &mesh) const
{
    /* Select VAO */
    glBindVertexArray(mesh.m_vao);

    /* Upload VBO */
    glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
    GLint vbo = glGetAttribLocation(program, "inVertex");
    if (vbo < 0) // Attribute not found in shader
        throw std::runtime_error("\"inVertex\" was not found in shader");
    if (mesh.m_vertices == 0)
        throw std::runtime_error("Mesh has no vertices");

    glVertexAttribPointer(vbo, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vbo);

    /* Upload NBO */
    glBindBuffer(GL_ARRAY_BUFFER, mesh.m_nbo);
    GLint nbo = glGetAttribLocation(program, "inNormal");
    if (nbo < 0)
        throw std::runtime_error("\"inNormal\" was not found in shader");
    if (mesh.m_normals == 0)
        throw std::runtime_error("Mesh has no normals");

    glVertexAttribPointer(nbo, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(nbo);

    /* Draw */
    if (mesh.m_indices == 0)
        glDrawArrays(GL_TRIANGLES, 0, mesh.m_vertices);
    else
        glDrawElements(GL_TRIANGLES, mesh.m_indices, GL_UNSIGNED_INT, 0);
}

void Renderer::drawModel(const Model &model) const
{
    const Shader &shader = model.shader();
    const Mesh &mesh = model.mesh();
    glUseProgram(shader.program);
    glm::mat4 mvp = m_camera->worldToViewToScreen() * model.modelToWorld();
    uploadMatrices(shader.program, mvp, glm::mat3());
    drawMesh(shader.program, mesh);
}

void Renderer::render(const std::vector<Model> &models)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (const auto &model : models)
    {
        drawModel(model);
    }
    glfwSwapBuffers(m_window);
}
