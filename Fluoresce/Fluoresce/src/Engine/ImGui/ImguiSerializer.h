//==============================================================================//
// Name : ImguiSerializer.h														// 
// Describe : 	Imgui�V���A���C�U												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Engine/ImGui/ImguiLayer.h"

namespace Fluoresce {

	// Imgui�V���A���C�U
	class ImguiSerializer
	{
	public:
		static void Serialize(const std::string& filepath, sint32 fontIndex, const ImguiLayer::CostomItemColor& colors);
		static bool Deserialize(const std::string& filepath);
	};

}