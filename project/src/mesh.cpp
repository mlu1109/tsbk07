#include "mesh.hpp"

Mesh Mesh::square()
{
    const std::vector<GLfloat> vertices{
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        -0.5, 0.5, 0.0};

    const std::vector<GLfloat> normals{
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0};

    const std::vector<GLuint> indices{
        0, 1, 2,
        0, 2, 3};

    return Mesh{vertices, normals, indices, std::vector<GLfloat>{}};
}

Mesh Mesh::triangle()
{
    const std::vector<GLfloat> vertices{
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

    const std::vector<GLfloat> normals{
        0.0, 0.5, 0.0,
        0.0, 0.5, 0.0,
        0.0, 0.5, 0.0};

    return Mesh{vertices, normals, std::vector<GLuint>(), std::vector<GLfloat>{}};
}

Mesh Mesh::cube()
{
    const std::vector<GLfloat> vertices{
        // front
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        // back
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5};

    
    const GLfloat n = 0.57735026919;
    const std::vector<GLfloat> normals{
        // front
        -n, -n, n,
        n, -n, n,
        n, n, n,
        -n, n, n,
        // back
        -n, -n, -n,
        n, -n, -n,
        n, n, -n,
        -n, n, -n};
    
    /*
    const std::vector<GLfloat> normals {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 0
    };
    */

    const std::vector<GLuint> indices{
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 0, 3,
        3, 7, 4,
        // left
        4, 5, 1,
        1, 0, 4,
        // right
        3, 2, 6,
        6, 7, 3};

    return Mesh{vertices, normals, indices, std::vector<GLfloat>{}};
}

Mesh::Mesh(const std::vector<GLfloat> &vertices,
           const std::vector<GLfloat> &normals,
           const std::vector<GLuint> &indices,
           const std::vector<GLfloat> &texCoords)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    m_vertices = vertices.size();
    if (0 != m_vertices)
    {
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    }

    m_normals = normals.size();
    if (0 != m_normals)
    {
        glGenBuffers(1, &m_nbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    }

    m_indices = indices.size();
    if (0 != m_indices)
    {
        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    m_texCoords = texCoords.size();
    if (0 != m_texCoords)
    {
        glGenBuffers(1, &m_tbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
    }
}

Mesh::~Mesh()
{

}