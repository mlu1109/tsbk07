#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "shader.hpp"

std::string readFile(const std::string &path)
{
    std::ifstream fs;
    fs.open(path.c_str());
    if (!fs)
        throw std::runtime_error("readFile failed file not found: " + path);

    std::stringstream ss;
    ss << fs.rdbuf();
    fs.close();
    return ss.str();
}

GLuint loadShaders(const std::string &vertPath, const std::string &fragPath)
{
    std::string vertexSource = readFile(vertPath);
    std::string fragmentSource = readFile(fragPath);

    /* Vertex Shader */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *source = (const GLchar *)vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        /* Compilation failed, print error and return */
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength); // The maxLength includes the NULL character
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        std::cout << &infoLog[0] << '\n';
        // We don't need the shader anymore.
        glDeleteShader(vertexShader);
        return 0;
    }

    /* Vertex Shader */
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = (const GLchar *)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);

    isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        /* Compilation failed, print error and return */
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength); // The maxLength includes the NULL character
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        std::cout << &infoLog[0] << '\n';
        // We don't need the shaders anymore.
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength); // The maxLength includes the NULL character
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        std::cout << &infoLog[0] << '\n';
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

std::unordered_map<ShaderType, Shader> initShaders()
{
    ShaderType type = ShaderType::colorNormal;
    std::string vertPath = "src/shaders/color_normal.vert";
    std::string fragPath = "src/shaders/color_normal.frag";
    GLuint program = loadShaders(vertPath, fragPath);
    Shader shader {
        program,
        vertPath,
        fragPath,
        true,
        true,
        false
    };

    return {
        {type, shader}
    };
}