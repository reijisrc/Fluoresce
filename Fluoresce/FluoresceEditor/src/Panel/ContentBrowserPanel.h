//==============================================================================//
// Name : ContentBrowserPanel.h													// 
// Describe : 	コンテンツブラウザパネル										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/08														// 
// Modify Date : 2023/01/08														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		enum class FileType : uint32
		{
			Directory = 0,
			SceneFile,
			TextureFile,
			UnknownFile,
		};

		struct FileInfo
		{
			std::string Name;
			FileType Type;
			std::filesystem::path Path;

			FileInfo() = default;

			FileInfo(const std::string& name, FileType type, const std::filesystem::path& path)
				: Name(name), Type(type), Path(path)
			{

			}
		};

		// コンテンツブラウザパネル
		class ContentBrowserPanel
		{
		public:
			using FilesInfo = std::vector<FileInfo>;
		public:
			ContentBrowserPanel();

			void Init();

			void OnImGuiRender();
		private:
			void GetDirectiryFiles();
		private:
			std::filesystem::path m_CurrentDirectory;
			FilesInfo				m_Files;

			Ref<Texture2D>			m_RootIcon;
			Ref<Texture2D>			m_ReloadIcon;
			Ref<Texture2D>			m_DirectiryIcon;
			Ref<Texture2D>			m_TexFileIcon;
			Ref<Texture2D>			m_SceneFileIcon;
			Ref<Texture2D>			m_FileIcon;
		};
	}

}