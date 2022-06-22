#pragma once
#include "render.h"
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 pos;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void render(RenderInfo &ri);

    void generate_mesh();

private:
    std::vector<Vertex> m_verts;
    std::vector<unsigned int> m_indices;

    unsigned int m_vao, m_vb, m_ib;
};

