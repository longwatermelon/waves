#pragma once
#include <glm/glm.hpp>

class Source
{
public:
    Source(glm::vec3 orig, float a, float k, float w);
    ~Source();

    float height_at(glm::vec3 pos) const;

private:
    glm::vec3 m_orig;
    float m_a, m_k, m_w;
};

