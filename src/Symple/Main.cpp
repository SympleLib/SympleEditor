#include <cstdio>
#include <Windows.h>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return 1;

    window = glfwCreateWindow(1280, 720, "Hello World", null, null);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

#if !__SY_DEBUG
    FreeConsole();
#endif

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
        return 2;

    puts((const char*)glGetString(GL_VERSION));

    glClearColor(.25f, .25f, .25f, 1.0f);

    float vertices[] = {
        -.5f,  .5f,
         .5f,  .5f,
         .5f, -.5f,
        -.5f, -.5f,
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), null);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_QUADS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}