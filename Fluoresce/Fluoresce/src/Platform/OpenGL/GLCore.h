//==============================================================================//
// Name : GLCore.h																// 
// Describe : 	GLÉRÉA															// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsCore.h"

namespace Fluoresce 
{
	class GLCore : public GraphicsCore
	{
		virtual void Init() override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void Clear() override;

		virtual void SetLineWidth(float32 width) override;
	};
}