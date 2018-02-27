#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <vector>

struct Mesh
{
    GLuint m_vao; // Vertex Array Object
    GLuint m_vbo; // Vertex Buffer Object
    GLuint m_nbo; // Normal Buffer Object
    GLuint m_ibo; // Index Buffer Object
    GLuint m_tbo; // TexCoord Buffer Object

    GLsizei m_vertices;
    GLsizei m_normals;
    GLsizei m_indices;
    GLsizei m_texCoords;

    static Mesh square();
    static Mesh triangle();
    static Mesh cube();

    Mesh(const std::vector<GLfloat> &vertices,
         const std::vector<GLfloat> &normals,
         const std::vector<GLuint> &indices,
         const std::vector<GLfloat> &texCoords);

    ~Mesh();
};

#endif