//==============================================================================//
// Name : TextureDefine.h														// 
// Describe :	テクスチャ定義													// 
// Author : Ding Qi																// 
// Create Date : 2023/02/23														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
		RGBA16f = 4,
	};

	enum class TextureFilter
	{
		Nearest = 1,
		Linear = 2,
	};

	enum class TextureWrap
	{
		Clamp = 0,
		Repeat = 1,
		MirroredRepeat = 2,
		Board = 3
	};

}