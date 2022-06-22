#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


Mesh::Mesh()
{
    generate_mesh();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vb);
    glGenBuffers(1, &m_ib);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(Vertex), m_verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_ib);
}


void Mesh::update(const std::vector<Source> &sources)
{
    /* for (size_t i = 0; i < m_verts.size(); ++i) */
    /*     m_verts[i].pos.y += .01f; */

    for (auto &v : m_verts)
    {
        float h = 0.f;
        glm::vec3 pos = v.pos;
        pos.y = 0.f;

        for (auto &s : sources)
            h += s.height_at(pos);

        v.pos.y = h;
    }

    for (size_t i = 0; i < m_indices.size(); ++i)
    {
        auto normal = [&](size_t a, size_t b) {
            if (a >= m_verts.size() || b >= m_verts.size() || a < 0 || b < 0)
                return glm::vec3(0.f, 0.f, 0.f);

            return glm::cross(m_verts[a].pos, m_verts[b].pos);
        };

        int s = 200;
        size_t idx = m_indices[i];

        glm::vec3 normals[8] = {
            normal(idx - s - 1, idx - 1),
            normal(idx - s, idx - s - 1),
            normal(idx - s + 1, idx - s),
            normal(idx + 1, idx - s + 1),
            normal(idx + s + 1, idx + 1),
            normal(idx + s, idx + s + 1),
            normal(idx + s - 1, idx + s),
            normal(idx - 1, idx + s - 1)
        };

        glm::vec3 avg(0.f, 0.f, 0.f);

        for (int j = 0; j < 8; ++j)
            avg += normals[j];

        avg /= 8.f;

        m_verts[idx].norm = avg;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(Vertex), m_verts.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::render(RenderInfo &ri)
{
    glm::mat4 model(1.f);
    shader_mat4(ri.shader, std::string("model"), &model[0][0]);
    shader_mat4(ri.shader, std::string("view"), &ri.cam->view()[0][0]);
    shader_mat4(ri.shader, std::string("projection"), &ri.proj[0][0]);

    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}


void Mesh::generate_mesh()
{
    int s = 200;

    for (int x = 0; x < s; ++x)
    {
        for (int z = 0; z < s; ++z)
        {
            m_verts.emplace_back(Vertex{
                glm::vec3((float)x / 10.f, -1.f, (float)z / 10.f),
                glm::vec3(0.f, 1.f, 0.f)
            });

            unsigned int i = x * s + z;

            if (x != s - 1 && z != s - 1)
            {
                unsigned int first[3] = { i, i + s, i + s + 1 };
                unsigned int second[3] = { i, i + 1, i + s + 1 };

                for (int k = 0; k < 3; ++k)
                    m_indices.emplace_back(first[k]);

                for (int k = 0; k < 3; ++k)
                    m_indices.emplace_back(second[k]);
            }
        }
    }
}

