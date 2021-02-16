#include <cstdio>
#include <Windows.h>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	while (!glfwWindowShouldClose(window))
	{
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

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
}