#include <Windows.h>

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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}