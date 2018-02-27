#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <string>
#include <GL/glew.h>
#include <unordered_map>

enum class ShaderType
{
    color,
    colorNormal,
};

struct Shader
{
    GLuint program;
    std::string vertPath;
    std::string fragPath;
    bool inVertex;
    bool inNormal;
    bool inTexCoord;
};

std::unordered_map<ShaderType, Shader> initShaders();

#endif