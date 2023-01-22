//==============================================================================//
// Name : ImguiLayer.h															// 
// Describe : 	Imguiレイヤー													// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Fluoresce {

	// Imguiレイヤー
	class ImguiLayer : public Layer
	{
	public:
		using CostomItemColor = std::array<FrVec4, 18>;
	public:
		ImguiLayer();
		~ImguiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void DockspaceBegin();
		void DockspaceEnd();

		void SetDefaultFont();
		void SetFont(sint32 index);
		sint32 GetFontIndex() const { return m_FontIndex; };

		void SetDefaultItemColor();
		void SetItemColor(const CostomItemColor& itemcolor);
		const CostomItemColor& GetCostomItemColor() const { return m_ItemColor; };
	private:
		bool m_BlockEvents = true;
		sint32	m_FontIndex = 0;
		CostomItemColor m_ItemColor;
	};
}