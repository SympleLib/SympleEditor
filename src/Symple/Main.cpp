#include <cstdio>
#include <Windows.h>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Symple/Typedefs.h"
#include "Symple/Window.h"

using namespace Symple;

#include <thread>

int main(void)
{
#if !__SY_DEBUG
	FreeConsole();
#endif
	if (!glfwInit())
		exit(2);

	Window win1;
	//Window win2;
	Window::StartLoop();

	glfwTerminate();
}