//==============================================================================//
// Name : EditorLayer.h															// 
// Describe : 	エディターレイヤー												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"
#include "UIWindow/MenuWindow.h"

namespace Fluoresce {

	namespace Editor
	{
		// エディターレイヤー
		class EditorLayer : public Layer
		{
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

		private:
			MenuWindow	m_MenuWindow;
		};
	}
};