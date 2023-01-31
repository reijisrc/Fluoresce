//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2023/01/07														// 
//==============================================================================//
#include "EditorLayer.h"
#include "EditorCore.h"

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

			//m_Texture = EditorCore::LoadTextureAsset("brickwall.jpg");

			m_Scene = CreateRef<Scene>();

			auto camera = m_Scene->CreateEntity("Camera");
			camera.AddComponent<CameraComponent>();
			auto sceneCamera = camera.GetComponent<CameraComponent>();
			sceneCamera.Camera.SetOrthographic(5.0f,-1.0f, 1.0f);

			auto square = m_Scene->CreateEntity("Sprite");
			square.AddComponent<SpriteRendererComponent>(Vec4{ 0.0f, 1.0f, 1.0f, 1.0f });


			m_SceneHierarchyPanel.SetContext(m_Scene);
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
			// ビューポートリサイズ
			if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
				(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
			{
				m_Framebuffer->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				resize = true;
			}

			// シーンリサイズ
			if (resize)
			{
				m_Scene->OnViewportResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
			}

			m_Scene->OnUpdate(ts);

			lineRenderer.ResetStats();
			spriteRenderer.ResetStats();

			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(m_ViewportClearColor);
			RenderCommand::Clear();

			// カラーバッファ1:クリア
			m_Framebuffer->ClearAttachment(1, -1);

			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			Vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			// マウスpick情報
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				sint32 pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = (pixelData == -1) ? Entity() : Entity(static_cast<entt::entity>(pixelData), m_Scene.get());
			}

			m_Scene->OnRender(ts);

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

				DrawPanels();

				pImguiLayer->DockspaceEnd();
			}
		}

		void EditorLayer::OnEvent(Event& e)
		{
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<KeyPressedEvent>(FR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
			dispatcher.Dispatch<MouseButtonPressedEvent>(FR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		}

		bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
		{
			return false;
		}

		bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
		{
			if (e.GetMouseButton() == Mouse::ButtonLeft)
			{
				if (m_ViewportHovered)
					m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
			return false;
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
					ImGui::MenuItem("SceneHierarchyPanel", NULL, &m_PanelFlag.at(EditorPanel::_EditorPanel_SceneHierarchyPanel));
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

		void EditorLayer::DrawPanels()
		{
			if (m_PanelFlag.at(EditorPanel::_EditorPanel_SceneHierarchyPanel))
			{
				m_SceneHierarchyPanel.OnImGuiRender();
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
					std::string name = "None";
					ImGui::ColorEdit4("BgColor", glm::value_ptr(m_ViewportClearColor));
					if (m_HoveredEntity)
						name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
					ImGui::Text("Hovered Entity: %s", name.c_str());
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