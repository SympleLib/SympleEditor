#pragma once

#include "Symple/Panel.h"

#include <GLFW/glfw3.h>

#include <SympleCode/Syntax/Lexer.h>

namespace Symple
{
	ImVec2 operator -(const ImVec2& l, const ImVec2& r)
	{ return ImVec2(l.x - r.x, l.y - r.y); }

	const char& GetChar(std::string_view str, uint32 col, uint32 ln)
	{
		for (uint32 l = 0; l < ln; l++)
			str = str.substr(str.find('\n') + 1);
		str = str.substr(std::min(col, str.length() - 1));
		return *str.data();
	}

	struct TextEditorPanel : Panel
	{
		char Text[4096] = "";
		std::string_view TextView = Text;
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

			shared_ptr<Syntax::Token> tok;
			Syntax::Lexer lexer(NULL, (std::string)TextView);
			while (!(tok = lexer.Lex())->Is(Syntax::Token::EndOfFile))
				Tokens.push_back(tok);

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
					shared_ptr<Syntax::Token> tok;
					Syntax::Lexer lexer(NULL, (std::string)Text);
					while (!(tok = lexer.Lex())->Is(Syntax::Token::EndOfFile))
						Tokens.push_back(tok);
				}
				ImGui::PopStyleColor();
				for (auto tok : Tokens)
				{
					ImVec4 col;
					switch (tok->GetKind())
					{
					case Syntax::Token::Unknown:
						col = ImVec4(1, 0, 0, 1);
						break;
					case Syntax::Token::Identifier:
						col = ImVec4(.75, .75, .75, 1);
						break;
					default:
						col = ImVec4(.75, .75, 1, 1);
						break;
					}

					if (tok->IsKeyword())
						col = ImVec4(1, 0, 1, 1);

					const char* c = &GetChar(TextView, tok->GetColumn(), tok->GetLine());
					ImGui::TextColored(col, "%s %c %c", tok->GetText().data(), c[tok->GetText().length() - 1]);
					ImGui::SameLine();
				}
				ImGui::PopStyleColor();
			};
		}
	};
}