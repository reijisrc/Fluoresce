//==============================================================================//
// Name : EditorCore.cpp														// 
// Describe : 	エディターコア													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "EditorCore.h"
#include "Engine/ImGui/ImguiSerializer.h"
#include "Engine/Scene/SceneSerializer.h"

#define EDITOR_VERSION               "0.71"

namespace Fluoresce {

	namespace Editor
	{
		static EditorState s_State = EditorState::Edit;

		static std::filesystem::path s_RootPath = "";
		static std::filesystem::path s_ResourcesPath = "";
		static std::filesystem::path s_ConfigPath = "";
		static std::filesystem::path s_UserDataPath = "";
		static std::filesystem::path s_AssetPath = "";
		static std::filesystem::path s_ScenePath = "";

		static std::filesystem::path s_CurrentScenePath = "";

		bool EditorCore::Init()
		{
			static const std::filesystem::path defaultResourcesPath = "resources";
			static const std::filesystem::path defaultConfigPath = "config";
			static const std::filesystem::path defaultUserDataPath = "userData";
			static const std::filesystem::path defaultAssetPath = "assets";
			static const std::filesystem::path defaultScenePath = "scene";

			// エディターパス初期化
			s_RootPath = std::filesystem::path(Application::Get().GetSpecification().WorkingDirectory.c_str());
			s_ResourcesPath = s_RootPath / defaultResourcesPath;
			s_ConfigPath = s_RootPath / defaultConfigPath;
			if (!std::filesystem::exists(s_ResourcesPath) || 
				!std::filesystem::exists(s_ConfigPath))
			{
				FR_ASSERT(false, "FluoresceEditor could not be initialized");
				return false;
			}
			else
			{
				s_UserDataPath = s_RootPath / defaultUserDataPath;
				create_directory(s_UserDataPath);

				s_AssetPath = s_UserDataPath / defaultAssetPath;
				s_ScenePath = s_UserDataPath / defaultScenePath;
				s_CurrentScenePath = std::filesystem::path();
				if (!std::filesystem::exists(s_UserDataPath))
				{
					create_directory(s_UserDataPath);
					create_directory(s_AssetPath);
				}

				// Imguiカスタマイズ設定初期化
				ImguiSerializer serializer;
				if (serializer.Deserialize(EditorCore::GetConfigPath()))
				{
					return true;
				}
				// デフォルト設定
				else
				{
					if (auto pImguiLayer = Application::Get().GetImguiLayer(); pImguiLayer)
					{
						pImguiLayer->SetDefaultFont();
						pImguiLayer->SetDefaultItemColor();
					}
				}

				return true;
			}
		}

		void EditorCore::ShutDown()
		{

		}

		const char* EditorCore::GetVersion()
		{
			return EDITOR_VERSION;
		}

		void EditorCore::SceneSerialize(const Ref<Scene>& scene, const std::string& filepath, const std::string& scenename)
		{
			SceneSerializer serializer(scene);
			serializer.Serialize(filepath, scenename);
		}

		bool EditorCore::SceneDeserialize(const Ref<Scene>& scene, const std::string& filepath)
		{
			SceneSerializer serializer(scene);
			return serializer.Deserialize(filepath);
		}

		EditorState EditorCore::GetEditorState()
		{
			return s_State;
		}

		void EditorCore::SetEditorState(EditorState state)
		{
			s_State = state;
		}

		std::filesystem::path& EditorCore::GetPath(EditorPath path)
		{
			switch (path)
			{
			case Fluoresce::Editor::EditorPath::_EditorResourcesPath:
				return s_ResourcesPath;
			case Fluoresce::Editor::EditorPath::_EditorUserDataPath:
				return s_UserDataPath;
			case Fluoresce::Editor::EditorPath::_EditorConfigPath:
				return s_ConfigPath;
			case Fluoresce::Editor::EditorPath::_EditorScenePath:
				return s_ScenePath;
			case Fluoresce::Editor::EditorPath::_AssetsPath:
				return s_AssetPath;
			default:
				return s_RootPath;
			}
		}

		std::filesystem::path& EditorCore::GetCurrentScenePath()
		{
			return s_CurrentScenePath;
		}

		void EditorCore::SetCurrentScenePath(const std::filesystem::path& path)
		{
			s_CurrentScenePath = path;
		}

		const std::string EditorCore::GetConfigPath()
		{
			std::string path = s_ConfigPath.string();
			path += "/editor.config";

			return path;
		}

		const std::string EditorCore::GetDragDropPayloadStr(DragDropPayloadType type)
		{
			switch (type)
			{
			case DragDropPayloadType::_Undefined:
				return std::string();
			case DragDropPayloadType::_SceneFile:
				return std::string("CONTENT_ITEM_SCENE");
			case DragDropPayloadType::_TextureFile:
				return std::string("CONTENT_ITEM_TEXTURE");
			case DragDropPayloadType::_TextureAsset:
				return std::string("ASSET_ITEM_TEXTURE");
			default:
				return std::string();
			}
		}

		Ref<Texture2D> EditorCore::LoadTextureAsset(const std::string& path)
		{
			std::string texturePath = EditorCore::GetPath(EditorPath::_AssetsPath).string();
			texturePath += "/textures/";
			texturePath += path;
			return Texture2D::Create(texturePath);
		}
	}
}
