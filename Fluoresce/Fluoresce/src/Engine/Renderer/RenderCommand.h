//==============================================================================//
// Name : RenderCommand.h														// 
// Describe :	レンダーコマンド												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsCore.h"

namespace Fluoresce {

	// レンダーコマンド
	class RenderCommand
	{
	public:
		static void Init()
		{
			s_GraphicsCore->Init();
		}

		static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
		{
			s_GraphicsCore->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const FrVec4& color)
		{
			s_GraphicsCore->SetClearColor(color);
		}

		static void SetClearColor(const Vec4& color)
		{
			s_GraphicsCore->SetClearColor(color);
		}

		static void Clear(ClearCommand command = ClearCommand::ClearCommand_Default)
		{
			s_GraphicsCore->Clear(command);
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 count = 0)
		{
			s_GraphicsCore->DrawIndexed(vertexArray, count);
		}

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32 vertexCount)
		{
			s_GraphicsCore->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float32 width)
		{
			s_GraphicsCore->SetLineWidth(width);
		}

		static void SetDepthTest(bool enable)
		{
			s_GraphicsCore->SetDepthTest(enable);
		}

		static void SetDepthTestFunc(DepthTestFunc func)
		{
			s_GraphicsCore->SetDepthTestFunc(func);
		}

		static void SetMemoryBarrier(GPUMemoryBarrier barriers)
		{
			s_GraphicsCore->SetMemoryBarrier(barriers);
		}
	private:
		static Scope<GraphicsCore> s_GraphicsCore;
	};
};