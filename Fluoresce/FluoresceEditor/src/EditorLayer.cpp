//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#include "EditorLayer.h"
#include "EditorCore.h"

#include "Engine/Graphics/GraphicsCore.h"
#include <imgui.h>

namespace Fluoresce {

	namespace Editor
	{
		EditorLayer::EditorLayer() :
			Layer("EditorLayer")
		{
			for (uint32 i = 0; i < EditorPanel::_EditorPanel_Max; i++)
			{
				m_PanelFlag.at(i) = true;
			}
		}

		EditorLayer::~EditorLayer()
		{
		}

		void EditorLayer::OnAttach()
		{
			FramebufferSpecification fbSpec;
			fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			fbSpec.Width = 1280;
			fbSpec.Height = 720;
			m_Framebuffer = Framebuffer::Create(fbSpec);

			m_Texture = EditorCore::LoadTextureAsset("brickwall.jpg");
		}

		void EditorLayer::OnDetach()
		{
		}

		void EditorLayer::OnUpdate(DeltaTime ts)
		{
			static const Camera camera(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f));

			auto& lineRenderer = RenderPipeline::GetLineRenderer();
			auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

			bool resize = false;
			// ビューポットリサイズ
			if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
				(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
			{
				m_Framebuffer->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				resize = true;
			}

			lineRenderer.ResetStats();
			spriteRenderer.ResetStats();

			m_Framebuffer->Bind();
			RenderCommand::Clear();
			RenderCommand::SetClearColor(m_ViewportColor);

			spriteRenderer.Begin(camera, Mat4(1.0f));
			spriteRenderer.DrawSprite(Vec3(0.0f, 0.0f, 0.0f), Vec2(0.5f, 0.5f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), m_Texture);
			spriteRenderer.End();

			lineRenderer.Begin(camera, Mat4(1.0f));
			lineRenderer.DrawRect(Vec3(0.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(0.0f, 1.0f, 1.0f, 1.0f));
			lineRenderer.End();

			m_Framebuffer->Unbind();
		}

		void EditorLayer::OnImguiRender()
		{
			if (auto pImguiLayer = Application::Get().GetImguiLayer(); pImguiLayer)
			{
				pImguiLayer->DockspaceBegin();

				DrawMenuBar();

				DrawViewport();

				DrawSceneSettings();

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
					ImGui::MenuItem("Viewport", NULL, &m_PanelFlag.at(EditorPanel::_EditorPanel_Viewport));
					ImGui::MenuItem("SceneSetting", NULL, &m_PanelFlag.at(EditorPanel::_EditorPanel_SceneSettings));
					ImGui::Separator();
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

		void EditorLayer::DrawViewport()
		{
			if (m_PanelFlag.at(EditorPanel::_EditorPanel_Viewport))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport", &m_PanelFlag.at(EditorPanel::_EditorPanel_Viewport));
				{
					auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
					auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
					auto viewportOffset = ImGui::GetWindowPos();
					m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
					m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

					m_ViewportFocused = ImGui::IsWindowFocused();
					m_ViewportHovered = ImGui::IsWindowHovered();
					Application::Get().GetImguiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

					ImVec2 viewport = ImGui::GetContentRegionAvail();
					m_ViewportSize = { viewport.x, viewport.y };
					uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
					ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				}
				ImGui::End();
				ImGui::PopStyleVar();
			}
		}

		void EditorLayer::DrawSceneSettings()
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			if (m_PanelFlag.at(EditorPanel::_EditorPanel_SceneSettings))
			{
				auto& lineRenderer = RenderPipeline::GetLineRenderer();
				auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				ImGui::Begin("ScenePanel", &m_PanelFlag.at(EditorPanel::_EditorPanel_SceneSettings));

				if (ImGui::TreeNodeEx((void*)2754597, treeNodeFlags, "Scene Setting"))
				{
					ImGui::ColorEdit4("BgColor", glm::value_ptr(m_ViewportColor));
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx((void*)3512625, treeNodeFlags, "Renderer Stats"))
				{
					auto& lineRenderer = RenderPipeline::GetLineRenderer();
					auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

					auto linestats = lineRenderer.GetStats();
					ImGui::Text("LineRenderer Stats:");
					ImGui::Text("Line Draw Calls: %d", linestats.DrawCalls);
					ImGui::Text("Line Vertices: %d", linestats.VertexCount);
					ImGui::Text("Line Indices: %d", linestats.IndexCount);

					auto spritestats = spriteRenderer.GetStats();
					ImGui::Text("SpriteRenderer Stats:");
					ImGui::Text("Sprite Draw Calls: %d", spritestats.DrawCalls);
					ImGui::Text("Sprite Vertices: %d", spritestats.VertexCount);
					ImGui::Text("Sprite Indices: %d", spritestats.IndexCount);

					ImGui::TreePop();
				}

				ImGui::End();
				ImGui::PopStyleVar();
			}
		}
	}
}