#include <cstdio>
#include <Windows.h>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Symple/Typedefs.h"
#include "Symple/Panel.h"

ImVec2 operator -(const ImVec2& l, const ImVec2& r)
{ return ImVec2(l.x - r.x, l.y - r.y); }

int main(void)
{
	using namespace Symple;
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiWindowFlags_HorizontalScrollbar;

	io.FontDefault = io.Fonts->AddFontFromFileTTF("res/fonts/CascadiaCode.ttf", 16);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	struct TextEditorPanel : Panel
	{
		char Text[4096] = "";
		std::string Filename;
		bool Edited = false;

		TextEditorPanel(const std::string& filename = "new")
			: Panel(filename), Filename(filename)
		{
			DrawFn = [this]()
			{
				ImVec2 min = ImGui::GetWindowContentRegionMin();
				ImVec2 max = ImGui::GetWindowContentRegionMax();
				
				if (ImGui::InputTextMultiline("##TextEditor.Text.0", Text, sizeof(Text), max - min) && !Edited)
				{
					Title = Filename + '*';
				}

				if ((ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)) && ImGui::IsKeyDown(GLFW_KEY_S))
				{
					FILE* fs = fopen(Filename.c_str(), "wb");
					fputs(Text, fs);
					fclose(fs);

					Edited = false;
					Title = Filename;
				}
			};
		}
	} textEditor("test/Main.c");

	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::BeginMainMenuBar();
		//ImGui::BeginMenu("File");
		if (ImGui::MenuItem("Exit", "Escape") || ImGui::IsKeyReleased(GLFW_KEY_ESCAPE))
			break;
		//ImGui::EndMenu();
		ImGui::EndMainMenuBar();

		textEditor.Draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* backupWin = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupWin);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
}