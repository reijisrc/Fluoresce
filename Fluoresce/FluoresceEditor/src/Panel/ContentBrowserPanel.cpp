//==============================================================================//
// Name : ContentBrowserPanel.cpp												// 
// Describe : 	コンテンツブラウザパネル										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/08														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "Panel/ContentBrowserPanel.h"

#include "EditorCore.h"
#include <imgui.h>

namespace Fluoresce {

	namespace Editor
	{
		static const std::string s_RootIconPath = "icons/home.png";
		static const std::string s_ReloadIconPath = "icons/refresh.png";
		static const std::string s_DirectiryIconPath = "icons/folder.png";
		static const std::string s_TexFileIconPath = "icons/image.png";
		static const std::string s_SceneFileIconPath = "icons/file-text.png";
		static const std::string s_DefFileIconPath = "icons/file.png";

		static const std::string GetDragDropPayloadStrByType(FileType type)
		{
			switch (type)
			{
			case FileType::SceneFile:
				return EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_SceneFile);
			case FileType::TextureFile:
				return EditorCore::GetDragDropPayloadStr(DragDropPayloadType::_TextureFile);
			default:
				return "";
			}
		}

		ContentBrowserPanel::ContentBrowserPanel()
		{

		}

		void ContentBrowserPanel::Init()
		{
			std::string resourcespath = EditorCore::GetPath(EditorPath::_EditorResourcesPath).string();
			resourcespath += "/";

			m_RootIcon = Texture2D::Create(resourcespath + s_RootIconPath);
			m_ReloadIcon = Texture2D::Create(resourcespath + s_ReloadIconPath);
			m_DirectiryIcon = Texture2D::Create(resourcespath + s_DirectiryIconPath);
			m_TexFileIcon = Texture2D::Create(resourcespath + s_TexFileIconPath);
			m_SceneFileIcon = Texture2D::Create(resourcespath + s_SceneFileIconPath);
			m_FileIcon = Texture2D::Create(resourcespath + s_DefFileIconPath);

			m_CurrentDirectory = EditorCore::GetPath(EditorPath::_EditorUserDataPath);
			GetDirectiryFiles();
		}

		void ContentBrowserPanel::OnImGuiRender()
		{
			static const std::string pathStr("Path: ");
			static bool reloadFlag = true;
			static float32 iconSize = 24.0f;
			static float32 padding = 16.0f;
			static float32 thumbnailSize = 64.0f;

			ImGui::Begin("Content Browser");
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((ImTextureID)(uint64)m_RootIcon->GetRendererID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
			{
				m_CurrentDirectory = EditorCore::GetPath(EditorPath::_EditorUserDataPath);
				reloadFlag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(uint64)m_ReloadIcon->GetRendererID(), { iconSize, iconSize }, { 0, 1 }, { 1, 0 }))
			{
				reloadFlag = true;
			}
			//ImGui::PopStyleColor(1);
			ImGui::SameLine();
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.15f));
			std::string path = pathStr + m_CurrentDirectory.string();
			ImGui::Button(path.c_str(), { 0, iconSize });
			ImGui::Separator();
			ImGui::PopStyleColor(1);
			float32 cellSize = thumbnailSize + padding;

			float32 panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (sint32)(panelWidth / cellSize);
			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (m_CurrentDirectory != EditorCore::GetPath(EditorPath::_EditorUserDataPath))
			{
				ImGui::ImageButton((ImTextureID)(uint64)m_DirectiryIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					m_CurrentDirectory = m_CurrentDirectory.parent_path();
					reloadFlag = true;
				}

				ImGui::TextWrapped("..");
				ImGui::NextColumn();
			}

			for (auto& file : m_Files)
			{
				const auto& path = file.Path;

				Ref<Texture2D> icon;
				switch (file.Type)
				{
				case FileType::Directory:
					icon = m_DirectiryIcon;
					break;
				case FileType::SceneFile:
					icon = m_SceneFileIcon;
					break;
				case FileType::TextureFile:
					icon = m_TexFileIcon;
					break;
				case FileType::UnknownFile:
					icon = m_FileIcon;
					break;
				default:
					break;
				}

				ImGui::PushID(file.Name.c_str());
				ImGui::ImageButton((ImTextureID)(uint64)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if (file.Type != FileType::Directory)
				{
					if (ImGui::BeginDragDropSource())
					{
						const wchar_t* itemPath = path.c_str();
						auto dropItemStr = GetDragDropPayloadStrByType(file.Type);
						ImGui::SetDragDropPayload(dropItemStr.c_str(), itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						ImGui::EndDragDropSource();
					}
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (file.Type == FileType::Directory)
					{
						auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);
						m_CurrentDirectory /= relativePath;
						reloadFlag = true;
					}
				}

				ImGui::TextWrapped(file.Name.c_str());

				ImGui::NextColumn();
				ImGui::PopID();
			}

			ImGui::PopStyleColor();
			ImGui::Columns(1);
			ImGui::End();

			if (reloadFlag)
			{
				GetDirectiryFiles();
				reloadFlag = false;
			}
		}

		void ContentBrowserPanel::GetDirectiryFiles()
		{
			m_Files.clear();
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();
				auto relativePath = std::filesystem::relative(path, EditorCore::GetPath(EditorPath::_EditorUserDataPath));
				std::string filenameString = relativePath.filename().string();
				std::string fileformatString = relativePath.extension().string();
				FileType type = FileType::Directory;
				if (!directoryEntry.is_directory())
				{
					if (fileformatString == ".scene")
					{
						type = FileType::SceneFile;
					}
					else if (fileformatString == ".png" || fileformatString == ".jpg")
					{
						type = FileType::TextureFile;
					}
					else
					{
						type = FileType::UnknownFile;
					}
				}

				m_Files.push_back({ filenameString, type, path });
			}

			std::sort(m_Files.begin(), m_Files.end(), [](const FileInfo& lhs, const FileInfo& rhs)
			{
				if (lhs.Type == rhs.Type)
				{
					return lhs.Name < rhs.Name;
				}
				else
				{
					return  lhs.Type < rhs.Type;
				}
			});
		}
	}

}