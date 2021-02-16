#include "Window.h"

#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Symple::Window
{
	GLFWwindow* New(const std::string& title, uint32 width, uint32 height, GLFWmonitor* monitor, GLFWwindow* share)
	{
		GLFWwindow* win = glfwCreateWindow(width, height, title.c_str(), monitor, share);
		if (win)
			return win;
		else
		{
			glfwTerminate();
			exit(-1);
		}
	}

	void Delete(GLFWwindow* win)
	{
		glfwSetWindowShouldClose(win, GL_TRUE);
		glfwDestroyWindow(win);
	}


	void BeginDraw(GLFWwindow*);
	void EndDraw();
}