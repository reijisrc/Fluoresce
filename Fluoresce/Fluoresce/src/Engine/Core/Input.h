//==============================================================================//
// Name : Input.h																// 
// Describe :	ƒ‚ƒWƒ…[ƒ‹:“ü—Í													// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Fluoresce {

	// “ü—Í
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}