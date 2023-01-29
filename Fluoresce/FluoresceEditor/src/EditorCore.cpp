//==============================================================================//
// Name : EditorCore.cpp														// 
// Describe : 	エディターコア													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "EditorCore.h"
#include "Engine/ImGui/ImguiSerializer.h"

#define EDITOR_VERSION               "0.31"

namespace Fluoresce {

	namespace Editor
	{
		static std::filesystem::path s_RootPath = "";
		static std::filesystem::path s_ResourcesPath = "";
		static std::filesystem::path s_ConfigPath = "";
		static std::filesystem::path s_UserDataPath = "";
		static std::filesystem::path s_AssetPath = "";

		bool EditorCore::Init()
		{
			static const std::filesystem::path defaultResourcesPath = "resources";
			static const std::filesystem::path defaultConfigPath = "config";
			static const std::filesystem::path defaultUserDataPath = "userData";
			static const std::filesystem::path defaultAssetPath = "assets";

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
			case Fluoresce::Editor::EditorPath::_AssetsPath:
				return s_AssetPath;
			default:
				return s_RootPath;
			}
		}

		const std::string EditorCore::GetConfigPath()
		{
			std::string path = s_ConfigPath.string();
			path += "/editor.config";

			return path;
		}
	}
}
