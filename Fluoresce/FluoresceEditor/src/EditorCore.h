//==============================================================================//
// Name : EditorCore.h															// 
// Describe : 	�G�f�B�^�[�R�A													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
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

		// �G�f�B�^�[�R�A
		class EditorCore
		{
		public:
			static bool Init();
			static void ShutDown();

			static const char* GetVersion();

			static std::filesystem::path& GetPath(EditorPath path);

			static const std::string GetConfigPath();
		};
	}
};
