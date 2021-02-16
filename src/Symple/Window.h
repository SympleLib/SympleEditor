#pragma once

#include <string>
#include <list>
#include <functional>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <imgui.h>

#include "Symple/Typedefs.h"

namespace Symple
{
	class Window
	{
	private:
		GLFWwindow* mWindow;
		std::string mTitle;
		uint32 mWidth, mHeight;

		std::function<void(const Window*)> mUpdateFn = [](const Window* win) {};

		static std::list<const Window*> sWindows;
	public:
		Window(const std::string& title = "Untitled Window", uint32 width = 1280, uint32 height = 720);
		~Window();

		static void StartLoop();
		void SetUpdateFunc(std::function<void(const Window*)>);
	};
}