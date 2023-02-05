//==============================================================================//
// Name : AssetsPanel.cpp														// 
// Describe : 	アセットパネル													// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "Panel/AssetsPanel.h"

#include "EditorCore.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Fluoresce {

	namespace Editor
	{
		AssetsPanel::AssetsPanel() :
			m_TextureSelection(0)
		{

		}

		void AssetsPanel::ClearTextureAssets()
		{
			m_TextureSelection = 0;
			m_TexturesInfo.clear();
			AssetsManager::Get().ReleaseAssets();
			RenderPipeline::RequestClearAllBatchAssets();
		}

		void AssetsPanel::ReloadTextureAssetInfo()
		{
			m_TextureSelection = 0;
			auto& textureAssets = AssetsManager::Get().GetTextureAssets();
			m_TexturesInfo.clear();
			if (!textureAssets.empty())
			{
				for (auto& texture : textureAssets) {
					m_TexturesInfo.push_back(texture.second.first.Name);
				}
			}
		}

		void AssetsPanel::OnImGuiRender()
		{
			static float32 textureSize = 256.0f;
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImGui::Begin("Assets");
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			if (ImGui::TreeNodeEx((void*)1756139, treeNodeFlags, "TextureAssets"))
			{
				auto& assetManager = AssetsManager::Get();

				if (ImGui::BeginListBox("##listbox 2", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
				{
					for (int n = 0; n < m_TexturesInfo.size(); n++)
					{
						const bool is_selected = (m_TextureSelection == n);
						if (ImGui::Selectable(m_TexturesInfo.at(n).c_str(), is_selected))
							m_TextureSelection = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				if (ImGui::IsItemHovered() && !m_TexturesInfo.empty())
				{
					if (ImGui::BeginDragDropSource())
					{
						std::string dropItemStr = EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_TextureAsset);
						auto dropTexNameStr = m_TexturesInfo.at(m_TextureSelection);
						ImGui::SetDragDropPayload(dropItemStr.c_str(), dropTexNameStr.c_str(), dropTexNameStr.size() * sizeof(char));
						ImGui::EndDragDropSource();
					}
				}
				else
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_TextureFile).c_str()))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = path;
							std::string filenameString = texturePath.stem().string();
							assetManager.LoadTexture(texturePath.string(), filenameString);
							m_TexturesInfo.push_back(filenameString);
						}
						ImGui::EndDragDropTarget();
					}
				}

				if (m_TextureSelection < m_TexturesInfo.size())
				{
					auto currentName = m_TexturesInfo.at(m_TextureSelection).c_str();
					ImGui::Text("Name: %s", currentName);
					auto wTexture = assetManager.GetTextureInstance(currentName);
					if (!wTexture.expired())
					{
						auto tex = wTexture.lock();
						ImGui::Text("Width: %d", tex->GetWidth());
						ImGui::SameLine();
						ImGui::Text("Height: %d", tex->GetWidth());
						ImGui::ImageButton((ImTextureID)(uint64)tex->GetRendererID(), { textureSize , textureSize }, { 0, 1 }, { 1, 0 });
					}
				}

				ImGui::Separator();

				if (ImGui::Button("Clear TextureAssets"))
				{
					ClearTextureAssets();
				}

				ImGui::TreePop();
			}
			ImGui::PopStyleVar();
			ImGui::End();
		}
	}
}