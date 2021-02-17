#pragma once

#include "Symple/Panel.h"

#include <GLFW/glfw3.h>

#include <SympleCode/Syntax/Lexer.h>

namespace Symple
{
	ImVec2 operator -(const ImVec2& l, const ImVec2& r)
	{ return ImVec2(l.x - r.x, l.y - r.y); }

	struct TextEditorPanel : Panel
	{
		char Text[4096] = "";
		std::string Filename;
		bool Edited = false;

		Syntax::TokenList Tokens;

		TextEditorPanel(const std::string& filename = "new")
			: Panel(filename + "###" + filename), Filename(filename)
		{
			FILE* fs = fopen(Filename.c_str(), "rb");
			if (fs)
			{
				fgets(Text, sizeof(Text), fs);
				fclose(fs);
			}

			DrawFn = [this]()
			{
				ImVec2 min = ImGui::GetWindowContentRegionMin();
				ImVec2 max = ImGui::GetWindowContentRegionMax(); max.y /= 2;

				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(.15, .15, .15, 1));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
				if (ImGui::InputTextMultiline("##TextEditor.Text.0", Text, sizeof(Text), max - min))
				{
					if (!Edited)
						Title = Filename + "*###" + Filename;

					Tokens.clear();
					shared_ptr<Syntax::Token> tok;
					unique_ptr<Syntax::Lexer> lexer = make_unique<Syntax::Lexer>((char*)"<NA>", std::string(Text));
					while (!(tok = lexer->Lex())->Is(Syntax::Token::EndOfFile))
						Tokens.push_back(tok);
					lexer.release();
				}
				ImGui::PopStyleColor();
				for (auto tok : Tokens)
				{
					ImGui::TextColored(tok->GetKind() > Syntax::Token::VoidKeyword ? ImVec4(1, 0, 1, 1) : ImVec4(.75, .75, .75, 1), "%s ", tok->GetText().data());
				}
				ImGui::PopStyleColor();

				if ((ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)) && ImGui::IsKeyDown(GLFW_KEY_S))
				{
					FILE* fs = fopen(Filename.c_str(), "wb");
					if (fs)
					{
						fputs(Text, fs);
						fclose(fs);

						Edited = false;
						Title = Filename + "###" + Filename;
					}
				}
			};
		}
	};
}