//==============================================================================//
// Name : EditorLayer.h															// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2023/01/09														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"
#include "UIWindow/MenuWindow.h"

namespace Fluoresce {

	namespace Editor
	{
		enum EditorPanel : uint32
		{
			_EditorPanel_Viewport = 0,
			_EditorPanel_SceneSettings,
			_EditorPanel_SceneHierarchyPanel,
			_EditorPanel_ContentBrowserPanel,
			_EditorPanel_Max
		};

		// エディターレイヤー
		class EditorLayer : public Layer
		{
		public:
			using PanelFlag = std::array<bool, EditorPanel::_EditorPanel_Max>;
		public:
			EditorLayer();
			virtual ~EditorLayer();

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnUpdate(DeltaTime ts) override;
			virtual void OnImguiRender() override;
			virtual void OnEvent(Event& e) override;

		private:
			bool OnKeyPressed(KeyPressedEvent& e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

			void DuplicateEntity();

			void DrawMenuBar();
			void DrawPanels();
			void DrawViewport();
			void DrawSceneSettings();

			void NewScene();
			void OpenScene();
			void OpenScene(const std::filesystem::path& path);
			void SaveScene();
			void SaveSceneAs();
		private:
			bool m_ViewportFocused = false;
			bool m_ViewportHovered = false;
			Vec2 m_ViewportSize = { 0.0f, 0.0f };
			Vec2 m_ViewportBounds[2] =
			{
				{ 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			};
			Vec4 m_ViewportClearColor = { 0.1f, 0.1f ,0.1f, 1.0f };

			Ref<Framebuffer> m_Framebuffer;
			Ref<Scene>		m_Scene;

			EditorCamera m_EditorCamera;
			Entity		m_HoveredEntity;
			PanelFlag	m_PanelFlag;

			SceneHierarchyPanel m_SceneHierarchyPanel;
			ContentBrowserPanel m_ContentBrowserPanel;
			MenuWindow			m_MenuWindow;
		};
	}
};