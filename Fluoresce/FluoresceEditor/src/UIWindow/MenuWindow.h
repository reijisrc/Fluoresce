//==============================================================================//
// Name : MenuWindow.h															// 
// Describe : 	���j���[�E�C���h�E												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		// ���j���[�E�C���h�E
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