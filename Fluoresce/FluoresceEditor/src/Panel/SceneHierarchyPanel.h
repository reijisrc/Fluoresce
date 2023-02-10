//==============================================================================//
// Name : SceneHierarchyPanel.h													// 
// Describe : 	シーンヒエラルキーパネル										// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2023/01/09														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"
#include "Scene/EditorScene.h"

namespace Fluoresce {

	namespace Editor
	{
		// シーンヒエラルキーパネル
		class SceneHierarchyPanel
		{
		public:
			SceneHierarchyPanel() = default;
			SceneHierarchyPanel(const  Ref<EditorScene>& scene);

			void SetContext(const Ref<EditorScene>& scene);

			void OnImGuiRender();

			Entity GetSelectedEntity() const { return m_SelectionContext; }
			void SetSelectedEntity(Entity entity);
		private:
			template<typename T>
			void DisplayAddComponentEntry(const std::string& entryName);

			void DrawEntityNode(Entity entity);
			void DrawComponents(Entity entity);
		private:
			Ref<EditorScene> m_Context;
			Entity m_SelectionContext;
		};

	}
};