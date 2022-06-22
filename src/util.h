#ifndef UTIL_H
#define UTIL_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

namespace util
{
    std::string read_file(const char *path);

    glm::quat quat_from_rot(glm::vec3 rot);
}

#endif

