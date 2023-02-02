//==============================================================================//
// Name : ImguiUtil.h															// 
// Describe : 	Imgui汎用機能													// 
// Author : Ding Qi																// 
// Create Date : 2023/01/22														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		// Imgui汎用機能
		class ImguiUtil
		{
		public:
			static bool DrawVec2Controller(const std::string& label, Vec2& values, float32 resetValue = 0.0f, float32 columnWidth = 100.0f);
			static bool DrawVec3Controller(const std::string& label, Vec3& values, float32 resetValue = 0.0f, float32 columnWidth = 100.0f);
		};
	}
}