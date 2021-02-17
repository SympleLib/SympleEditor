#pragma once

#include <functional>

#include <imgui.h>

namespace Symple
{
	typedef std::function<void()> DrawingFn;

	void PanelDefaultDrawFn() {}

	struct Panel
	{
		std::string Title;
		bool POpen = true;
		DrawingFn DrawFn = PanelDefaultDrawFn;

		Panel(const std::string& title = "Unnamed Panel", DrawingFn drawFn = PanelDefaultDrawFn, bool pOpen = true)
			: Title(title), DrawFn(drawFn), POpen(pOpen) {}
		~Panel() {}

		void Draw()
		{
			if (!POpen)
				return;

			if (ImGui::Begin(Title.c_str(), &POpen))
				DrawFn();
			ImGui::End();
		}
	};
}