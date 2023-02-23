//==============================================================================//
// Name : EditorLayer.cpp														// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#include "EditorLayer.h"
#include "EditorCore.h"

#include "Engine/Utils/FileUtil.h"
#include "Engine/Utils/GlmUtil.h"

#include "ImguiUtil/ImguiUI.h"
#include "NativeScript/ScriptTaskHandle.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Fluoresce {

	namespace Editor
	{
		static const std::string s_PlayIconPath = "icons/play.png";
		static const std::string s_StopIconPath = "icons/stop.png";
		static const std::string s_PauseIconPath = "icons/pause.png";

		EditorLayer::EditorLayer() :
			Layer("EditorLayer")
		{
			for (uint32 i = 0; i < EditorPanel::_Panel_Max; i++)
			{
				m_PanelFlag.at(i) = true;
			}
		}

		EditorLayer::~EditorLayer()
		{
		}

		void EditorLayer::OnAttach()
		{
			{
				std::string resourcespath = EditorCore::GetPath(EditorPath::_EditorResourcesPath).string();
				resourcespath += "/";
				m_IconPlay = Texture2D::Create(resourcespath + s_PlayIconPath);
				m_IconStop = Texture2D::Create(resourcespath + s_StopIconPath);
				m_IconPause = Texture2D::Create(resourcespath + s_PauseIconPath);
			}

			m_ContentBrowserPanel.Init();

			m_InvertColorShader = ComputeShader::Create("resources/shaders/Grayscale_Comp.glsl");

			// MSAAフレームバッファ
			FramebufferSpecification baseFBSpec;
			baseFBSpec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
			baseFBSpec.Width = 1280;
			baseFBSpec.Height = 720;
			baseFBSpec.Samples = m_ViewportSamples;
			m_HDRBuffer = Framebuffer::Create(baseFBSpec);

			// MSAAコピー用中間バッファ
			FramebufferSpecification intermediateFBSpec;
			intermediateFBSpec.Attachments = { FramebufferTextureFormat::RGBA16F };
			intermediateFBSpec.Width = 1280;
			intermediateFBSpec.Height = 720;
			m_IntermediateBuffer = Framebuffer::Create(intermediateFBSpec);

			// ポストプロセスバッファ
			FramebufferSpecification postprocessingSpec;
			postprocessingSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
			postprocessingSpec.Width = 1280;
			postprocessingSpec.Height = 720;
			m_PostProcessingBuffer = Framebuffer::Create(postprocessingSpec);

			m_EditorCamera = EditorCamera(30.0f, 1600.0f / 900.0f, 0.1f, 1000.0f);

			m_EditorScene = CreateRef<EditorScene>();
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
		}

		void EditorLayer::OnDetach()
		{
		}

		void EditorLayer::OnUpdate(DeltaTime ts)
		{
			auto& postProcessingRenderer = RenderPipeline::GetPostProcessingRenderer();

			bool resize = false;
			// ビューポートリサイズ
			if (FramebufferSpecification spec = m_HDRBuffer->GetSpecification();
				m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
				(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
			{
				m_HDRBuffer->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				m_IntermediateBuffer->Resize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				resize = true;
			}

			switch (EditorCore::GetEditorState())
			{
			case EditorState::Edit:
				if (resize)
				{
					m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
					m_EditorScene->OnViewportResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				}
				m_EditorCamera.OnUpdate(ts);
				m_EditorScene->OnEditorUpdate(ts);
				break;
			case EditorState::Runtime:
				if (resize)
				{
					m_RuntimeScene->OnViewportResize((uint32)m_ViewportSize.x, (uint32)m_ViewportSize.y);
				}
				m_RuntimeScene->OnRuntimeUpdate(ts);
				break;
			}

			RenderPipeline::ResetAllBatchStats();

			m_HDRBuffer->Bind();

			switch (EditorCore::GetEditorState())
			{
			case EditorState::Edit:
				m_EditorScene->OnEditorRender(ts, m_EditorCamera);
				break;
			case EditorState::Runtime:
				m_RuntimeScene->OnRuntimeRender(ts);
				break;
			}

			// MSAAコピー
			if (m_ViewportSamples > 1)
			{
				m_HDRBuffer->BlitMultisampledBuffer(m_IntermediateBuffer);
				m_HDRBuffer->Unbind();
			}


			auto& baseBuffer = (m_ViewportSamples > 1) ? m_IntermediateBuffer : m_HDRBuffer;

			// コンピュートシェーダ処理段階
			if (m_GrayScale)
			{
				m_InvertColorShader->Bind();
				m_InvertColorShader->BindImageTexture(0, baseBuffer->GetColorAttachmentRendererID(0), 0, ImageTextureAccessFlag::Read, TextureFormat::RGBA16f);
				m_InvertColorShader->BindImageTexture(1, baseBuffer->GetColorAttachmentRendererID(0), 0, ImageTextureAccessFlag::Write, TextureFormat::RGBA16f);
				m_InvertColorShader->DispatchCompute(static_cast<uint32>(m_ViewportSize.x / 16.0f), static_cast<uint32>(m_ViewportSize.y / 9.0f), 1);
				RenderCommand::SetMemoryBarrier(MemoryBarrierOption::TextureBarriers);
			}

			// ポストプロセス
			m_PostProcessingBuffer->Bind();
			postProcessingRenderer.Submit(baseBuffer, m_Exposure);
			m_PostProcessingBuffer->Unbind();
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
			if (EditorCore::GetEditorState() == EditorState::Edit)
			{
				m_EditorCamera.OnEvent(e);
			}

			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<KeyPressedEvent>(FR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
			dispatcher.Dispatch<MouseButtonPressedEvent>(FR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		}

		bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
		{
			// ショットカット
			if (e.IsRepeat())
				return false;

			bool editmode = (EditorCore::GetEditorState() == EditorState::Edit) ? true : false;
			bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

			switch (e.GetKeyCode())
			{
			case Key::N:
			{
				if (control)
					NewScene();
				return true;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				return true;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
					{
						SaveSceneAs();
					}
					else
					{
						SaveScene();
					}
				}
				return true;
			}
			case Key::D:
			{
				if (control)
					DuplicateEntity();

				break;
			}
			// ImGizmos
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing() && editmode)
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing() && editmode)
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing() && editmode)
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing() && editmode)
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			}
			return false;
		}

		bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
		{
			if (e.GetMouseButton() == Mouse::ButtonLeft)
			{
				if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				{
					// MSAA pick使えない
					// m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
				}
			}
			return false;
		}

		void EditorLayer::OnScenePlay()
		{
			EditorCore::SetEditorState(EditorState::Runtime);
			m_GizmoType = -1;

			m_RuntimeScene = EditorScene::Copy(m_EditorScene);
			
			SceneScriptTask::Get().BuildNativeScript(m_RuntimeScene, ScriptTaskHandle::BindScript);
			m_RuntimeScene->OnRuntimeStart();

			m_SceneHierarchyPanel.SetContext(m_RuntimeScene);
		}

		void EditorLayer::OnSceneStop()
		{
			EditorCore::SetEditorState(EditorState::Edit);

			m_RuntimeScene->OnRuntimeStop();

			m_SceneHierarchyPanel.SetContext(m_EditorScene);
		}

		void EditorLayer::DuplicateEntity()
		{
			if (EditorCore::GetEditorState() != EditorState::Edit)
				return;

			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
				m_EditorScene->DuplicateEntity(selectedEntity);
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
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save", "Ctrl+S"))
					{
						SaveScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						SaveSceneAs();
					}

					if (ImGui::MenuItem("Exit"))
					{
						Fluoresce::Application::Get().Close();
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Setting"))
				{
					ImGui::MenuItem("Viewport", NULL, &m_PanelFlag.at(EditorPanel::_Viewport));
					ImGui::MenuItem("SceneSetting", NULL, &m_PanelFlag.at(EditorPanel::_Panel_SceneSettings));
					ImGui::MenuItem("SceneHierarchyPanel", NULL, &m_PanelFlag.at(EditorPanel::_Panel_SceneHierarchyPanel));
					ImGui::MenuItem("ContentBrowserPanell", NULL, &m_PanelFlag.at(EditorPanel::_Panel_ContentBrowserPanel));
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
			if (m_PanelFlag.at(EditorPanel::_Panel_SceneHierarchyPanel))
			{
				m_SceneHierarchyPanel.OnImGuiRender();
			}

			if (m_PanelFlag.at(EditorPanel::_Panel_AssetsPanel))
			{
				m_AssetsPanel.OnImGuiRender();
			}

			if (m_PanelFlag.at(EditorPanel::_Panel_ContentBrowserPanel))
			{
				m_ContentBrowserPanel.OnImGuiRender();
			}
		}

		void EditorLayer::DrawViewport()
		{
			if (m_PanelFlag.at(EditorPanel::_Viewport))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("Viewport", &m_PanelFlag.at(EditorPanel::_Viewport));
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
					uint64_t textureID = m_PostProcessingBuffer->GetColorAttachmentRendererID();
					ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				}

				// ドロップシーン
				if (EditorCore::GetEditorState() == EditorState::Edit)
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_SceneFile).c_str()))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							OpenScene(path);
						}
						ImGui::EndDragDropTarget();
					}
				}

				// ImGizmos
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity && m_GizmoType != -1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
					
					const Mat4& cameraProjection = m_EditorCamera.GetProjection();
					Mat4 cameraView = m_EditorCamera.GetViewMatrix();

					auto& tc = selectedEntity.GetComponent<TransformComponent>();
					Mat4 transform = tc.GetTransform();

					// Snapping
					bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale

					// Snap to 45 degrees for rotation
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;
					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_GizmoType,
						ImGuizmo::LOCAL,
						glm::value_ptr(transform),
						nullptr,
						snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						Vec3 translation, rotation, scale;
						Fluoresce::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - tc.Rotation;
						tc.Translation = translation;
						tc.Rotation += deltaRotation;
						tc.Scale = scale;
					}
				}

				ImGui::End();
				ImGui::PopStyleVar();
			}
		}

		void EditorLayer::DrawSceneSettings()
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			if (m_PanelFlag.at(EditorPanel::_Panel_SceneSettings))
			{
				auto& lineRenderer = RenderPipeline::GetLineRenderer();
				auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				ImGui::Begin("ScenePanel", &m_PanelFlag.at(EditorPanel::_Panel_SceneSettings));
				Ref<Texture2D> icon = (EditorCore::GetEditorState() == EditorState::Edit) ? m_IconPlay : m_IconStop;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton((ImTextureID)(uint64)icon->GetRendererID(), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), 0))
				{
					if (EditorCore::GetEditorState() == EditorState::Edit)
						OnScenePlay();
					else if (EditorCore::GetEditorState() == EditorState::Runtime)
						OnSceneStop();
				}
				if (EditorCore::GetEditorState() == EditorState::Runtime)
				{
					ImGui::SameLine();
					if (ImGui::ImageButton((ImTextureID)(uint64)m_IconPause->GetRendererID(), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), 0))
					{
						m_RuntimeScene->IsPaused() ? m_RuntimeScene->SetPaused(false) : m_RuntimeScene->SetPaused(true);
					}
				}
				ImGui::PopStyleColor(1);
				ImGui::Separator();

				if (ImGui::TreeNodeEx((void*)2754597, ImGuiTreeNodeFlags_DefaultOpen | treeNodeFlags, "Scene Setting"))
				{
					ImGui::Text("ViewportWidth:  %.2f", m_ViewportSize.x);
					ImGui::Text("ViewportHeight: %.2f", m_ViewportSize.y);
					ImGui::Checkbox("GrayScale", &m_GrayScale);
					ImGui::DragFloat("Exposure", &m_Exposure, 0.01f, 0.0f, 10.0f);
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx((void*)5654532, ImGuiTreeNodeFlags_DefaultOpen | treeNodeFlags, "Main Camera"))
				{
					if (EditorCore::GetEditorState() == EditorState::Edit)
					{
						Vec3 pos = m_EditorCamera.GetFocusPoint();
						if (ImguiUI::DrawVec3Controller("FocusPoint", pos, 0.0f, 80.0f))
						{
							m_EditorCamera.SetFocusPoint(pos);
						}
						ImGui::Separator();
						float32 dist = m_EditorCamera.GetDistance();
						if (ImGui::DragFloat("Distance", &dist, 1.0f, m_EditorCamera.GetNearClip(), m_EditorCamera.GetFarClip()))
						{
							m_EditorCamera.SetDistance(dist);
						}
					}
					else if (EditorCore::GetEditorState() == EditorState::Runtime)
					{
						bool moveComExist = false;
						if (auto cameraEntity = m_RuntimeScene->GetPrimaryCameraEntity(); cameraEntity)
						{
							if (cameraEntity.HasComponent<TransformComponent>())
							{
								auto& transComponent = cameraEntity.GetComponent<TransformComponent>();
								ImguiUI::DrawVec3Controller("Position", transComponent.Translation, 0.0f, 80.0f);
								ImGui::Separator();
								moveComExist = true;
							}
						}
						
						if(!moveComExist)
						{
							ImGui::Text("Null");
						}
					}
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

		void EditorLayer::NewScene()
		{
			m_EditorScene = CreateRef<EditorScene>();
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.ResetView(1600.0f / 900.0f);
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			m_AssetsPanel.ClearTextureAssets();
			m_AssetsPanel.ReloadTextureAssetInfo();

			EditorCore::SetCurrentScenePath(std::filesystem::path());
		}

		void EditorLayer::OpenScene()
		{
			std::string filepath = Fluoresce::FileUtil::OpenFile("Fluoresce Scene (*.scene)\0*.scene\0");
			if (!filepath.empty())
			{
				OpenScene(filepath);
			}
		}

		void EditorLayer::OpenScene(const std::filesystem::path& path)
		{
			if (EditorCore::GetEditorState() == EditorState::Runtime)
				OnSceneStop();

			if (path.extension().string() != ".scene")
			{
				FR_CLIENT_WARN("Could not load {0} - not a scene file", path.filename().string());
				return;
			}

			Ref<EditorScene> newScene = CreateRef<EditorScene>();
			m_AssetsPanel.ClearTextureAssets();
			if (EditorCore::SceneDeserialize(newScene, path.string()))
			{
				m_EditorScene = newScene;
				m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.ResetView(1600.0f / 900.0f);
				m_SceneHierarchyPanel.SetContext(m_EditorScene);
				m_AssetsPanel.ReloadTextureAssetInfo();

				EditorCore::SetCurrentScenePath(path);
			}
		}

		void EditorLayer::SaveScene()
		{
			auto currentScenePath = EditorCore::GetCurrentScenePath();
			if (currentScenePath.empty())
			{
				auto relativePath = std::filesystem::relative(currentScenePath, EditorCore::GetPath(EditorPath::_EditorScenePath));
				std::string name = relativePath.stem().string();
				auto currentscene = (EditorCore::GetEditorState() == EditorState::Edit) ? m_EditorScene : m_RuntimeScene;
				EditorCore::SceneSerialize(currentscene, currentScenePath.string(), name);
			}
			else
			{
				SaveSceneAs();
			}
		}

		void EditorLayer::SaveSceneAs()
		{
			std::string filepath = Fluoresce::FileUtil::SaveFile("Fluoresce Scene (*.scene)\0*.scene\0");
			if (!filepath.empty())
			{
				std::filesystem::path path = filepath;
				auto relativePath = std::filesystem::relative(path, EditorCore::GetPath(EditorPath::_EditorScenePath));
				std::string name = relativePath.stem().string();
				auto currentscene = (EditorCore::GetEditorState() == EditorState::Edit) ? m_EditorScene : m_RuntimeScene;
				EditorCore::SceneSerialize(currentscene, filepath, name);
			}
		}
	}
}