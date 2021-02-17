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

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	struct TextEditorPanel : Panel
	{
		char Text[4096] = "";

		TextEditorPanel(const std::string& filename = "new")
			: Panel(filename + "##0")
		{
			DrawFn = [this]()
			{
				if (ImGui::InputTextMultiline("##TextEditor.Text.0", Text, sizeof(Text)))
				{
					char* c = Text;
					while (*c++) // lol
						if (*c == 'a')
							exit(*c);
				}
			};
		}
	} textEditor;

	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::BeginMainMenuBar();
		if (ImGui::MenuItem("Exit"))
			break;
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