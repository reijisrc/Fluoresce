//==============================================================================//
// Name : GLCore.h																// 
// Describe : 	GLコア															// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsCore.h"

namespace Fluoresce 
{
	// GLコア
	class GLCore : public GraphicsCore
	{
		virtual void Init() override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void SetClearColor(const FrVec4& color) override;
		virtual void SetClearColor(const Vec4& color) override;
		virtual void Clear() override;

		virtual void SetLineWidth(float32 width) override;
	};
}