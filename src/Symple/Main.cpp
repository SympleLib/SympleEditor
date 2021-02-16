#include <cstdio>
#include <Windows.h>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

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

    f32 vertices[] = {
        -.5f,  .5f,
         .5f,  .5f,
         .5f, -.5f,
        -.5f, -.5f,
    };

    u8 indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    u32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(f32), vertices, GL_STATIC_DRAW);

    u32 ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(u32), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), null);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 4 * 2, GL_UNSIGNED_BYTE, null);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}