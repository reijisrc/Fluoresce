//==============================================================================//
// Name : MenuWindow.cpp														// 
// Describe : 	メニューウインドウ												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "UIWindow/MenuWindow.h"
#include "EditorCore.h"

#include "Engine/ImGui/ImguiSerializer.h"

#include <imgui.h>

namespace Fluoresce {

	namespace Editor
	{
		MenuWindow::MenuWindow()
		{
			auto pImguiLayer = Application::Get().GetImguiLayer();
			if (pImguiLayer)
			{
				m_LocalFontIndex = pImguiLayer->GetFontIndex();
				m_LocalItemColor = pImguiLayer->GetCostomItemColor();
			}
		}

		void MenuWindow::DrawCustomizeWindow(bool* window)
		{
			static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
			static ImVec2 customizeSize = { 360.0f, 600.0f };

			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, customizeSize);
			ImGui::Begin("Customize", window, window_flags);
			if (ImGui::BeginTabBar("##CustomizeTab", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Font"))
				{
					DrawCustomizeFontSelector();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Colors"))
				{
					DrawCustomizeColorItem();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		
			{
				auto pImguiLayer = Application::Get().GetImguiLayer();

				if (ImGui::Button("Save"))
				{
					if (pImguiLayer)
					{
						ImguiSerializer serializer;
						serializer.Serialize(EditorCore::GetConfigPath(), m_LocalFontIndex, m_LocalItemColor);
					}
				}
				ImGui::SameLine();
				if(ImGui::Button("Default"))
				{
					if (pImguiLayer)
					{
						pImguiLayer->SetDefaultFont();
						pImguiLayer->SetDefaultItemColor();
						m_LocalFontIndex = pImguiLayer->GetFontIndex();
						m_LocalItemColor = pImguiLayer->GetCostomItemColor();
					}
				}
			}

			ImGui::PopStyleVar();
			ImGui::End();
		}

		void MenuWindow::DrawAboutWindow(bool* window)
		{
			static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
			static ImVec2 aboudSize = { 360.0f, 72.0f };

			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, aboudSize);
			ImGui::Begin("About", window, window_flags);
			ImGui::Text("  FluoresceEditor: %s", EditorCore::GetVersion());
			ImGui::Separator();
			ImGui::Text("  Copyright 2023 Ding Qi");
			ImGui::End();
			ImGui::PopStyleVar();
		}

		void MenuWindow::DrawCustomizeFontSelector()
		{
			ImGuiIO& io = ImGui::GetIO();
			ImFont* font_current = ImGui::GetFont();
			if (ImGui::BeginCombo("Font##MenuCustomize", font_current->GetDebugName()))
			{
				for (int n = 0; n < io.Fonts->Fonts.Size; n++)
				{
					ImFont* font = io.Fonts->Fonts[n];
					ImGui::PushID((void*)font);
					if (ImGui::Selectable(font->GetDebugName(), font == font_current))
					{
						m_LocalFontIndex = n;
						io.FontDefault = font;
					}
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
		}

		void MenuWindow::DrawCustomizeColorItem()
		{
			const static std::array<sint32, 18> colorItem[]
			{
				ImGuiCol_::ImGuiCol_WindowBg,
				ImGuiCol_::ImGuiCol_Header,
				ImGuiCol_::ImGuiCol_HeaderHovered,
				ImGuiCol_::ImGuiCol_HeaderActive,

				ImGuiCol_::ImGuiCol_Button,
				ImGuiCol_::ImGuiCol_ButtonHovered,
				ImGuiCol_::ImGuiCol_ButtonActive,

				ImGuiCol_::ImGuiCol_FrameBg,
				ImGuiCol_::ImGuiCol_FrameBgHovered,
				ImGuiCol_::ImGuiCol_FrameBgActive,

				ImGuiCol_::ImGuiCol_Tab,
				ImGuiCol_::ImGuiCol_TabHovered,
				ImGuiCol_::ImGuiCol_TabActive,
				ImGuiCol_::ImGuiCol_TabUnfocused,
				ImGuiCol_::ImGuiCol_TabUnfocusedActive,

				ImGuiCol_::ImGuiCol_TitleBg,
				ImGuiCol_::ImGuiCol_TitleBgActive,
				ImGuiCol_::ImGuiCol_TitleBgCollapsed
			};

			static ImVec2 customizeSize = { 0.0f, 500.0f };
			static ImGuiColorEditFlags colorItem_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;
			;
			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::BeginChild("##colors", customizeSize, false, ImGuiWindowFlags_NavFlattened);
			for (sint32 i = 0; i < colorItem->size(); i++)
			{
				auto index = colorItem->at(i);
				const char* name = ImGui::GetStyleColorName(index);
				ImGui::PushID(i);
				if (ImGui::ColorEdit4("##color", (float*)&style.Colors[index], colorItem_flags))
				{
					m_LocalItemColor.at(i) = FrVec4(style.Colors[index].x, style.Colors[index].y, style.Colors[index].z, style.Colors[index].w);
				}
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				ImGui::TextUnformatted(name);
				ImGui::PopID();
			};
			ImGui::EndChild();

		}

	}
}