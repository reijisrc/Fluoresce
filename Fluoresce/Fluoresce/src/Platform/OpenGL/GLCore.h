//==============================================================================//
// Name : GLCore.h																// 
// Describe : 	GLコア															// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2023/02/23														// 
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
		virtual void Clear(ClearCommand command) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float32 width) override;

		virtual void SetDepthTest(bool enable) override;
		virtual void SetDepthTestFunc(DepthTestFunc func) override;

		virtual void SetMemoryBarrier(GPUMemoryBarrier barriers) override;

		virtual const APICapabilities& GetCapabilities() const override;
	};
}