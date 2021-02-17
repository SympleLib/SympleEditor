#pragma once

#include "Symple/Panel.h"

#include <GLFW/glfw3.h>

#include <SympleCode/Syntax/Lexer.h>
#include <SympleCode/Util/FileUtil.h>

namespace Symple
{
	ImVec2 operator -(const ImVec2& l, const ImVec2& r)
	{ return ImVec2(l.x - r.x, l.y - r.y); }

	const char& GetChar(const char* str, uint32 col, uint32 ln)
	{
		std::string_view view = str;
		for (uint32 l = 0; l < ln; l++)
			view = view.substr(view.find('\n') + 1);
		if (view.length())
			view = view.substr(std::min(col, view.length() - 1));
		return *view.data();
	}

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
				strcpy(Text, Util::ReadFile(fs, 4096).c_str());
				fclose(fs);
			}

			shared_ptr<Syntax::Token> tok;
			Syntax::Lexer lexer(NULL, (std::string)Text);
			do
				Tokens.push_back(lexer.Lex());
			while (!Tokens.back()->Is(Syntax::Token::EndOfFile));

			DrawFn = [this]()
			{
				ImVec2 min = ImGui::GetWindowContentRegionMin();
				ImVec2 max = ImGui::GetWindowContentRegionMax(); max.y /= 2;

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

					return;
				}

				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(.15, .15, .15, 1));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
				if (ImGui::InputTextMultiline("##TextEditor.Text.0", Text, sizeof(Text), max - min))
				{
					if (!Edited)
						Title = Filename + "*###" + Filename;

					Tokens.clear();
					Syntax::Lexer lexer(NULL, (std::string)Text);
					do
						Tokens.push_back(lexer.Lex());
					while (!Tokens.back()->Is(Syntax::Token::EndOfFile));
				}
				ImGui::PopStyleColor();
				for (uint32 i = 0; i < Tokens.size() - 1; i++)
				{
					ImVec4 col;
					switch (Tokens[i]->GetKind())
					{
					case Syntax::Token::Unknown:
						col = ImVec4(1, 0, 0, 1);
						break;
					case Syntax::Token::Identifier:
						col = ImVec4(.75, .75, .75, 1);
						break;
					default:
						col = ImVec4(.25, 1, 1, 1);
						break;
					}

					if (Tokens[i]->IsKeyword())
						col = ImVec4(1, 0, 1, 1);

					const char* c = &GetChar(Text, Tokens[i]->GetColumn() + Tokens[i]->GetText().length(), Tokens[i]->GetLine());
					std::string trivia = c;
					int32 dist = &GetChar(Text, Tokens[i + 1]->GetColumn(), Tokens[i + 1]->GetLine()) - c + 1;
					printf("Dist: %i\n", dist);
					trivia.resize(dist);
					ImGui::TextColored(col, "%s%s", Tokens[i]->GetText().data(), trivia.c_str());
					ImGui::SameLine();
				}
				ImGui::PopStyleColor();
			};
		}
	};
}