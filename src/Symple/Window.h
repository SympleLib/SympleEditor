#pragma once

#include <string>
#include <functional>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <imgui.h>

#include "Symple/Typedefs.h"

namespace Symple::Window
{
	GLFWwindow* New(const std::string& title = "Untitled Window", uint32 width = 1280, uint32 height = 720, GLFWmonitor* = null, GLFWwindow* share = null);
	void Delete(GLFWwindow*);

	void BeginDraw(GLFWwindow*);
	void EndDraw();
}