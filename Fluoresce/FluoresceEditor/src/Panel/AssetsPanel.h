//==============================================================================//
// Name : AssetsPanel.h															// 
// Describe : 	アセットパネル													// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		class AssetsPanel
		{
		public:
			using TexturesInfoCache = std::vector<std::string>;
		public:
			AssetsPanel();

			void ClearTextureAssets();

			void ReloadTextureAssetInfo();

			void OnImGuiRender();
		private:
			sint32				m_TextureSelection;
			TexturesInfoCache	m_TexturesInfo;
		};
	}
}