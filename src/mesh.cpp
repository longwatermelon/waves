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


void Mesh::update()
{
    /* for (size_t i = 0; i < m_verts.size(); ++i) */
    /*     m_verts[i].pos.y += .01f; */

    float a = .4f;
    float k = 4.f;
    float w = .1f;

    glm::vec3 orig(10.f, 0.f, 10.f);

    for (size_t i = 0; i < m_verts.size(); ++i)
    {
        glm::vec3 pos = m_verts[i].pos;
        pos.y = 0.f;

        float h = a * sinf(k * glm::length(pos - orig) - w * (glfwGetTime() * 50.f));
        m_verts[i].pos.y = std::fmax(h, 0.f);
        /* m_verts[i].pos.y = h; */
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
            m_verts.emplace_back(Vertex{ glm::vec3((float)x / 10.f, -1.f, (float)z / 10.f) });

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

