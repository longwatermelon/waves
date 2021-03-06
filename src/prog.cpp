#include "prog.h"
#include "mesh.h"
#include "light.h"
#include <vector>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <stb/stb_image.h>


Prog::Prog(GLFWwindow *w)
    : m_win(w), m_cam(glm::vec3(0.f, 6.f, 20.f), glm::vec3(0.f, glm::radians(45.f), glm::radians(-30.f)))
{
    m_ri.add_shader("basic");
    m_ri.use_shader("basic");

    m_ri.proj = glm::perspective(glm::radians(45.f), 800.f / 600.f, .1f, 100.f);
    m_ri.cam = &m_cam;
}


Prog::~Prog()
{
    m_ri.clear_shaders();
}


void Prog::mainloop()
{
    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPos(m_win, 400.f, 300.f);
    glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double prev_mx, prev_my;
    glfwGetCursorPos(m_win, &prev_mx, &prev_my);

    Mesh m;
    std::vector<Source> sources = {
        Source(glm::vec3(5.f, 0.f, 10.f), .4f, 2.f, .1f, 0.f),
        Source(glm::vec3(15.f, 0.f, 10.f), .4f, 2.f, .1f, glm::radians(180.f))
    };

    while (!glfwWindowShouldClose(m_win))
    {
        events();

        double mx, my;
        glfwGetCursorPos(m_win, &mx, &my);
        m_cam.rotate(glm::vec3(0.f, -(mx - prev_mx) / 100.f, -(my - prev_my) / 100.f));

        prev_mx = mx;
        prev_my = my;

        m.update(sources);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ri.use_shader("basic");

        m_cam.set_props(m_ri);

        glm::mat4 model(1.f);
        shader_mat4(m_ri.shader, std::string("model"), &model[0][0]);
        shader_mat4(m_ri.shader, std::string("projection"), &m_ri.proj[0][0]);
        shader_mat4(m_ri.shader, std::string("view"), &m_cam.view()[0][0]);

        m.render(m_ri);

        glfwSwapBuffers(m_win);
        glfwPollEvents();
    }
}


void Prog::events()
{
    float move = .05f;

    glm::vec3 front = m_cam.front() * move;
    front[1] = 0.f;

    glm::vec3 right = m_cam.right() * move;
    right[1] = 0.f;

    if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS) m_cam.move(front);
    if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS) m_cam.move(-front);
    if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS) m_cam.move(-right);
    if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS) m_cam.move(right);

    if (glfwGetKey(m_win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) m_cam.move(glm::vec3(0.f, -move, 0.f));
    if (glfwGetKey(m_win, GLFW_KEY_SPACE) == GLFW_PRESS) m_cam.move(glm::vec3(0.f, move, 0.f));
}

