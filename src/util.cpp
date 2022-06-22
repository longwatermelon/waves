#include "util.h"
#include <sstream>
#include <fstream>
#include <iostream>


std::string util::read_file(const char *path)
{
    std::ifstream ifs(path);
    std::stringstream ss;
    std::string line;

    while (std::getline(ifs, line))
        ss << line << "\n";

    ifs.close();

    return ss.str();
}


glm::quat util::quat_from_rot(glm::vec3 rot)
{
    glm::quat yaw(glm::vec3(0.f, rot.y, 0.f));
    glm::quat pitch(glm::vec3(0.f, 0.f, rot.z));
    return glm::normalize(yaw * pitch);
}

