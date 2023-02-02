//==============================================================================//
// Name : EditorCore.h															// 
// Describe : 	エディターコア													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2023/02/01														// 
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
			_EditorScenePath,
			_AssetsPath,
		};

		enum class DragDropPayloadType
		{
			_Undefined = 0,
			_SceneFile,
			_TextureFile,
		};

		enum class EditorState
		{
			Edit = 0,
			Runtime = 1
		};

		// エディターコア
		class EditorCore
		{
		public:
			static bool Init();
			static void ShutDown();

			static const char* GetVersion();

			static void SceneSerialize(const Ref<Scene>& scene, const std::string& filepath, const std::string& scenename);
			static bool SceneDeserialize(const Ref<Scene>& scene, const std::string& filepath);

			static EditorState GetEditorState();
			static void SetEditorState(EditorState state);

			static std::filesystem::path& GetPath(EditorPath path);

			static std::filesystem::path& GetCurrentScenePath();
			static void SetCurrentScenePath(const std::filesystem::path& path);

			static const std::string GetConfigPath();
			static const std::string GetDragDropPayloadStr(DragDropPayloadType type);

			static Ref<Texture2D> LoadTextureAsset(const std::string& path);
		};
	}
};
