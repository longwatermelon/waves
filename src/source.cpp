#include "source.h"
#include <GLFW/glfw3.h>


Source::Source(glm::vec3 orig, float a, float k, float w)
    : m_orig(orig), m_a(a), m_k(k), m_w(w)
{
}


Source::~Source()
{
}


float Source::height_at(glm::vec3 pos) const
{
    float h = m_a * sinf(m_k * glm::length(pos - m_orig) - m_w * (glfwGetTime() * 50.f)); // height
    h *= powf(M_E, -glm::length(pos - m_orig) / 4.f); // damping
    h = std::fmax(h, 0.f);

    return h;
}

