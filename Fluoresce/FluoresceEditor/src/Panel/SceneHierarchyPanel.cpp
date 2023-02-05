//==============================================================================//
// Name : SceneHierarchyPanel.cpp												// 
// Describe : 	シーンヒエラルキーパネル										// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#include "Panel/SceneHierarchyPanel.h"

#include "Engine/Scene/Components.h"

#include "EditorCore.h"
#include "ImguiUtil/ImguiUI.h"
#include "NativeScript/ScriptTaskHandle.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Fluoresce {

	namespace Editor
	{
		template<typename T, typename UIFunction>
		static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			if (entity.HasComponent<T>())
			{
				auto& component = entity.GetComponent<T>();
				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
				ImGui::PopStyleVar();
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}

				if (open)
				{
					uiFunction(component);
					ImGui::TreePop();
				}

				if (removeComponent)
					entity.RemoveComponent<T>();
			}
		}

		SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		{
			SetContext(scene);
		}

		void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
		{
			m_Context = context;
			m_SelectionContext = {};
		}

		void SceneHierarchyPanel::OnImGuiRender()
		{
			const ImGuiPopupFlags menuflag = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
			ImGui::Begin("Scene Hierarchy");

			if (m_Context)
			{
				m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID , m_Context.get() };
					DrawEntityNode(entity);
				});

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					m_SelectionContext = {};

				if (ImGui::BeginPopupContextWindow(0, menuflag))
				{
					if (ImGui::MenuItem("Create Empty Entity"))
					{
						m_Context->CreateEntity("Empty Entity");
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();

			ImGui::Begin("Properties");
			if (m_SelectionContext)
				DrawComponents(m_SelectionContext);

			ImGui::End();
		}

		void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
		{
			m_SelectionContext = entity;
		}

		template<typename T>
		void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
			if (!m_SelectionContext.HasComponent<T>())
			{
				if (ImGui::MenuItem(entryName.c_str()))
				{
					m_SelectionContext.AddComponent<T>();
					ImGui::CloseCurrentPopup();
				}
			}
		}

		void SceneHierarchyPanel::DrawEntityNode(Entity entity)
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
			if (ImGui::IsItemClicked())
			{
				m_SelectionContext = entity;
			}

			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
					entityDeleted = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
				bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
				if (opened)
					ImGui::TreePop();
				ImGui::TreePop();
			}

			if (entityDeleted)
			{
				m_Context->DestroyEntity(entity);
				if (m_SelectionContext == entity)
					m_SelectionContext = {};
			}
		}

		void SceneHierarchyPanel::DrawComponents(Entity entity)
		{
			static sint32 textureSelection = 0;
			static sint32 scriptSelection = 0;

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

			if (entity.HasComponent<TagComponent>())
			{
				auto& tag = entity.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			}

			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				DisplayAddComponentEntry<CameraComponent>("Camera");
				DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
				DisplayAddComponentEntry<ScriptComponent>("Script");
				ImGui::EndPopup();
			}

			ImGui::PopItemWidth();

			DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				ImguiUI::DrawVec3Controller("Translation", component.Translation);
				Vec3 rotation = Vec3{ glm::degrees(component.Rotation.x),
						glm::degrees(component.Rotation.y),
						glm::degrees(component.Rotation.z)
				};
				if (ImguiUI::DrawVec3Controller("Rotation", rotation))
				{
					component.Rotation = Vec3{ glm::radians(rotation.x),
								glm::radians(rotation.y),
								glm::radians(rotation.z)
					};
				}
				ImguiUI::DrawVec3Controller("Scale", component.Scale, 1.0f);
			});

			DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::Checkbox("Visible", &component.Visible);
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::Button("Texture Slot", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_TextureFile).c_str()))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = path;
						std::string filename = texturePath.filename().string();
						if (auto tex = EditorCore::LoadTextureAsset(filename); tex)
						{
							component.TextureName = filename;
							component.Texture = tex;
							component.EnableTexture = true;
						}
						else
						{
							FR_CLIENT_WARN("Could not set texture {0}", filename);
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (component.Texture)
				{
					ImGui::Checkbox("Enable", &component.EnableTexture);
					ImGui::SameLine();
					ImGui::Text(": %s", component.TextureName.c_str());
				}
				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

			DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
			{
				bool runtime = (EditorCore::GetEditorState() == EditorState::Runtime) ? true : false;
				if (runtime)
				{
					ImGui::Text("Script: %s", ScriptTaskHandle::GetScriptName(component.ScriptID));
				}
				else
				{
					auto currentName = ScriptTaskHandle::GetScriptName(component.ScriptID);
					if (ImGui::BeginCombo("Script", currentName))
					{
						for (int n = 0; n < ScriptID::_ID_Max; n++)
						{
							auto name = ScriptTaskHandle::GetScriptName(n);
							ImGui::PushID((void*)name);
							if (ImGui::Selectable(name, name == currentName))
							{
								scriptSelection = n;
								component.ScriptID = scriptSelection;
							}
							ImGui::PopID();
						}
						ImGui::EndCombo();
					}
				}
			});
		}
	}
}