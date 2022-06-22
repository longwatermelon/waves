#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class Camera;

struct RenderInfo
{
    unsigned int shader;
    std::map<std::string, unsigned int> shaders;
    glm::mat4 proj;
    const Camera *cam;

    void add_shader(const std::string &name);
    void clear_shaders();

    void use_shader(const std::string &name);
};

