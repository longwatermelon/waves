#pragma once
#include "render.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Prog
{
public:
    Prog(GLFWwindow *w);
    ~Prog();

    void mainloop();
    void events();

private:
    GLFWwindow *m_win;
    RenderInfo m_ri;

    Camera m_cam;
};

