#include "Window.h"

#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Symple
{
	std::list<const Window*> Window::sWindows;

	Window::Window(const std::string& title, uint32 width, uint32 height)
		: mTitle(title), mWidth(width), mHeight(height)
	{
		mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), null, null);
		if (!mWindow)
		{
			glfwTerminate();
			exit(2);
		}

		glfwMakeContextCurrent(mWindow);
		if (glewInit() != GLEW_OK)
			exit(3);

		glViewport(0, 0, mWidth, mHeight);
		printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		ImGui_ImplOpenGL3_Init();

		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		sWindows.push_back(this);
	}

	Window::~Window()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

		sWindows.pop_back();
	}


	void Window::StartLoop()
	{
		while (true)
		{
			bool shouldClose = true;
			for (const auto* win : sWindows)
			{
				glfwMakeContextCurrent(win->mWindow);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				ImGui::DockSpaceOverViewport();

				if (ImGui::Begin("Text Editor"))
				{
					ImGui::Text("This is your code");
				}
				ImGui::End();

				if (ImGui::Begin("Test"))
				{
					ImGui::Text("Test");
				}
				ImGui::End();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(win->mWindow);
				glfwPollEvents();

				shouldClose = glfwWindowShouldClose(win->mWindow);
			}

			if (shouldClose)
				break;
		}
	}

	void Window::SetUpdateFunc(std::function<void(const Window*)> fn)
	{ mUpdateFn = fn; }
}