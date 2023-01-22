//==============================================================================//
// Name : MenuWindow.h															// 
// Describe : 	メニューウインドウ												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		// メニューウインドウ
		class MenuWindow
		{
		public:
			MenuWindow();

			void DrawCustomizeWindow(bool* window);
			void DrawAboutWindow(bool* window);
		private:
			void DrawCustomizeFontSelector();
			void DrawCustomizeColorItem();
		private:
			sint32	m_LocalFontIndex = 0;
		 	ImguiLayer::CostomItemColor m_LocalItemColor;
		};
	}
};