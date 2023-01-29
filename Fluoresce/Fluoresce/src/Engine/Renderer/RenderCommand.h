//==============================================================================//
// Name : RenderCommand.h														// 
// Describe :	�����_�[�R�}���h												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsCore.h"

namespace Fluoresce {

	// �����_�[�R�}���h
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

		static void Clear()
		{
			s_GraphicsCore->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_GraphicsCore->DrawIndexed(vertexArray, count);
		}

		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_GraphicsCore->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			s_GraphicsCore->SetLineWidth(width);
		}

	private:
		static Scope<GraphicsCore> s_GraphicsCore;
	};
};