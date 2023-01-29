//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "EditorLayer.h"

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
			static const Camera camera(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f));

			auto& lineRenderer = RenderPipeline::GetLineRenderer();
			auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

			lineRenderer.ResetStats();
			spriteRenderer.ResetStats();

			RenderCommand::Clear();
			RenderCommand::SetClearColor(clearColor);

			spriteRenderer.Begin(camera, Mat4(1.0f));
			spriteRenderer.DrawQuad(Vec3(0.0f, 0.0f, 0.0f), Vec2(0.25f, 0.25f), Vec4(0.0f, 1.0f, 1.0f, 1.0f));
			spriteRenderer.End();

			lineRenderer.Begin(camera, Mat4(1.0f));
			lineRenderer.DrawRect(Vec3(0.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 0.5f, 1.0f, 1.0f));
			lineRenderer.End();
		}

		void EditorLayer::OnImguiRender()
		{
			if (auto pImguiLayer = Application::Get().GetImguiLayer(); pImguiLayer)
			{
				//pImguiLayer->DockspaceBegin();

				//DrawMenuBar();
				//DrawRenderStats();

				//pImguiLayer->DockspaceEnd();
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

		void EditorLayer::DrawRenderStats()
		{
			auto& lineRenderer = RenderPipeline::GetLineRenderer();
			auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

			ImGui::Begin("Stats");

			auto linestats = lineRenderer.GetStats();
			ImGui::Text("LineRenderer Stats:");
			ImGui::Text("Draw Calls: %d", linestats.DrawCalls);
			ImGui::Text("QuadVertices: %d", linestats.VertexCount);
			ImGui::Text("QuadIndices: %d", linestats.IndexCount);

			auto spritestats = spriteRenderer.GetStats();
			ImGui::Text("SpriteRenderer Stats:");
			ImGui::Text("Draw Calls: %d", spritestats.DrawCalls);
			ImGui::Text("QuadVertices: %d", spritestats.VertexCount);
			ImGui::Text("QuadIndices: %d", spritestats.IndexCount);

			ImGui::End();
		}
	}
}