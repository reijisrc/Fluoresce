//==============================================================================//
// Name : EditorLayer.h															// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"
#include "Scene/EditorScene.h"
#include "Panel/AssetsPanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"
#include "UIWindow/MenuWindow.h"

namespace Fluoresce {

	namespace Editor
	{
		enum EditorPanel : uint32
		{
			_Viewport = 0,
			_Panel_SceneSettings,
			_Panel_SceneHierarchyPanel,
			_Panel_ContentBrowserPanel,
			_Panel_AssetsPanel,
			_Panel_Max
		};

		// エディターレイヤー
		class EditorLayer : public Layer
		{
		public:
			using PanelFlag = std::array<bool, EditorPanel::_Panel_Max>;
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

			void OnScenePlay();
			void OnSceneStop();
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
			uint32 m_ViewportSamples = 4;
			float32 m_Exposure = 1.0f;

			sint32 m_GizmoType = -1;

			Ref<Framebuffer> m_HDRBuffer;
			Ref<Framebuffer> m_IntermediateBuffer;
			Ref<Framebuffer> m_PostProcessingBuffer;

			Ref<EditorScene> m_EditorScene;
			Ref<EditorScene> m_RuntimeScene;

			Ref<Texture2D> m_IconPlay; 
			Ref<Texture2D> m_IconStop;
			Ref<Texture2D> m_IconPause;

			bool			   m_GrayScale = false;
			Ref<ComputeShader> m_InvertColorShader;

			EditorCamera m_EditorCamera;
			PanelFlag	m_PanelFlag;

			AssetsPanel			m_AssetsPanel;
			SceneHierarchyPanel m_SceneHierarchyPanel;
			ContentBrowserPanel m_ContentBrowserPanel;
			MenuWindow			m_MenuWindow;
		};
	}
};