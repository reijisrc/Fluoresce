//==============================================================================//
// Name : ImguiLayer.h															// 
// Describe : 	Imguiレイヤー													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Fluoresce {

	// Imguiレイヤー
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void DockspaceBegin();
		void DockspaceEnd();
	private:
		bool m_BlockEvents = true;
	};
}