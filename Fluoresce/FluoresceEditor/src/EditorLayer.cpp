﻿//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "EditorLayer.h"

//#include "EditorCore.h"
#include "Engine/Graphics/GraphicsCore.h"

#include <imgui.h>

namespace Fluoresce {

	namespace Editor
	{
		EditorLayer::EditorLayer() :
			Layer("EditorLayer")
		{
		}

		EditorLayer::~EditorLayer()
		{
		}

		void EditorLayer::OnAttach()
		{
		}

		void EditorLayer::OnDetach()
		{
		}

		void EditorLayer::OnUpdate(DeltaTime ts)
		{
			static const FrVec4 clearColor = FrVec4(0.8f, 0.25f, 0.4f, 1.0f);

			RenderCommand::Clear();
			RenderCommand::SetClearColor(clearColor);
		}

		void EditorLayer::OnImguiRender()
		{
			auto pImguiLayer = Application::Get().GetImguiLayer();

			if (pImguiLayer)
			{
				pImguiLayer->DockspaceBegin();

				DrawMenuBar();

				pImguiLayer->DockspaceEnd();
			}
		}

		void EditorLayer::OnEvent(Event& e)
		{

		}

		void EditorLayer::DrawMenuBar()
		{
			static bool showCustomizeWindow = false;
			static bool showAboutWindow = false;

			ImGuiIO& io = ImGui::GetIO();

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit"))
					{
						Fluoresce::Application::Get().Close();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Setting"))
				{
					ImGui::MenuItem("Customize", NULL, &showCustomizeWindow);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{
					ImGui::MenuItem("About", NULL, &showAboutWindow);
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			if (showCustomizeWindow)
			{
				m_MenuWindow.DrawCustomizeWindow(&showCustomizeWindow);
			}

			if (showAboutWindow)
			{
				m_MenuWindow.DrawAboutWindow(&showAboutWindow);
			}
		}
	}
}