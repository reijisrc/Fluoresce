//==============================================================================//
// Name : GraphicsCore.h														// 
// Describe : 	グラフィックスコア												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/VertexArray.h"

namespace Fluoresce {

	enum ClearCommand
	{
		CommandNone = 0,
		CommandClearColor = BIT(0),
		CommandClearDepth = BIT(1),
		CommandClearStencil = BIT(2),

		CommandDefault = (CommandClearColor | CommandClearDepth)
	};

	enum class DepthTestFunc
	{
		DepthTest_Default = 0,
		DepthTest_Always,
		DepthTest_Never,
		DepthTest_Less,
		DepthTest_Equal,
		DepthTest_Lequal,
		DepthTest_Greater,
		DepthTest_NotEqual,
		DepthTest_Gequal,
	};

	enum class GPUMemoryBarrier
	{
		Barrier_ShaderStorage,
		Barrier_Texture,
	};

	// グラフィックコア
	class GraphicsCore
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL4 = 1,
		};

		struct APICapabilities
		{
			sint32 MaxSamples = 0;
			float32 MaxAnisotropy = 0.0f;
			sint32 MaxTextureUnits = 0;
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		virtual void SetClearColor(const FrVec4& color) = 0;
		virtual void SetClearColor(const Vec4& color) = 0;
		virtual void Clear(ClearCommand command) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float32 width) = 0;

		virtual void SetDepthTest(bool enable) = 0;
		virtual void SetDepthTestFunc(DepthTestFunc func) = 0;

		virtual void SetMemoryBarrier(GPUMemoryBarrier barriers) = 0;

		virtual const APICapabilities& GetCapabilities() const = 0;

		static API GetAPI() { return s_API; }

		static Scope<GraphicsCore> Create();
	private:
		static API s_API;
	};
}