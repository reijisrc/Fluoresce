//==============================================================================//
// Name : EditorLayer.h															// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"
#include "UIWindow/MenuWindow.h"

namespace Fluoresce {

	namespace Editor
	{
		enum EditorPanel : uint32
		{
			_EditorPanel_Viewport = 0,
			_EditorPanel_SceneSettings,
			_EditorPanel_Max
		};

		// エディターレイヤー
		class EditorLayer : public Layer
		{
		public:
			using PanelFlag = std::array<bool, 2>;
		public:
			EditorLayer();
			virtual ~EditorLayer();

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnUpdate(DeltaTime ts) override;
			virtual void OnImguiRender() override;
			virtual void OnEvent(Event& e) override;

		private:
			void DrawMenuBar();

			void DrawViewport();
			void DrawSceneSettings();
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

			Ref<Texture2D>	m_Texture;
			Ref<Framebuffer> m_Framebuffer;
			Ref<Scene>		m_Scene;

			PanelFlag	m_PanelFlag;
			MenuWindow	m_MenuWindow;
		};
	}
};