//==============================================================================//
// Name : ImguiSerializer.h														// 
// Describe : 	Imguiシリアライザ												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#pragma once

#include "Engine/ImGui/ImguiLayer.h"

namespace Fluoresce {

	// Imguiシリアライザ
	class ImguiSerializer
	{
	public:
		static void Serialize(const std::string& filepath, sint32 fontIndex, const ImguiLayer::CostomItemColor& colors);
		static bool Deserialize(const std::string& filepath);
	};

}