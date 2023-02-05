//==============================================================================//
// Name : ImguiUI.h																// 
// Describe : 	Imgui”Ä—pUI														// 
// Author : Ding Qi																// 
// Create Date : 2023/01/22														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		// Imgui”Ä—pUI
		class ImguiUI
		{
		public:
			static bool DrawVec2Controller(const std::string& label, Vec2& values, float32 resetValue = 0.0f, float32 columnWidth = 100.0f);
			static bool DrawVec3Controller(const std::string& label, Vec3& values, float32 resetValue = 0.0f, float32 columnWidth = 100.0f);
		};
	}
}