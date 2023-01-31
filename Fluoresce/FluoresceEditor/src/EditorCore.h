//==============================================================================//
// Name : EditorCore.h															// 
// Describe : 	エディターコア													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2023/01/07														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

#include <filesystem>

namespace Fluoresce {

	namespace Editor
	{
		enum class EditorPath
		{
			_EditorResourcesPath,
			_EditorConfigPath,
			_EditorUserDataPath,
			_AssetsPath,
		};

		enum class DragDropPayloadType
		{
			_Undefined = 0,
			_TextureAsset,
		};


		// エディターコア
		class EditorCore
		{
		public:
			static bool Init();
			static void ShutDown();

			static const char* GetVersion();

			static std::filesystem::path& GetPath(EditorPath path);

			static const std::string GetConfigPath();

			static const std::string GetDragDropPayloadStr(DragDropPayloadType type);

			static Ref<Texture2D> LoadTextureAsset(const std::string& path);
		};
	}
};
