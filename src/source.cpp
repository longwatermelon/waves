#include "source.h"
#include <GLFW/glfw3.h>


Source::Source(glm::vec3 orig, float a, float k, float w, float phase)
    : m_orig(orig), m_a(a), m_k(k), m_w(w), m_phase(phase)
{
}


Source::~Source()
{
}


float Source::height_at(glm::vec3 pos) const
{
    float angle = m_k * glm::length(pos - m_orig) - m_w * (glfwGetTime() * 40.f) + m_phase;

    if (angle > 0.f)
        angle = 0.f;

    float h = m_a * sinf(angle); // height
    h *= powf(M_E, -glm::length(pos - m_orig) / 8.f); // damping

    return h;
}

