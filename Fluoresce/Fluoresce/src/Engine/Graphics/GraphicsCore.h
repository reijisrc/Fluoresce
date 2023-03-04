//==============================================================================//
// Name : GraphicsCore.h														// 
// Describe : 	グラフィックスコア												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/VertexArray.h"

namespace Fluoresce {

	// クリアコマンド
	enum ClearCommand
	{
		ClearCommand_None = 0,
		ClearCommand_Color = BIT(0),
		ClearCommand_Depth = BIT(1),
		ClearCommand_Stencil = BIT(2),

		ClearCommand_Default = (ClearCommand_Color | ClearCommand_Depth)
	};

	// ZバッファFunc
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

	// GPUメモリバリア
	enum class GPUMemoryBarrier
	{
		Barrier_ShaderStorage,
		Barrier_Texture,
	};

	// インダイレクト描画コマンド
	struct DrawIndirectCommand
	{
		uint32 VertexCount;
		uint32 InstanceCount;
		uint32 FirstIndex;
		uint32 BaseVertex;
		uint32 BaseInstance;
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
			sint32 MaxUniformBufferSize = 0;
			sint32 MaxTextureUnits = 0;
			float32 MaxAnisotropy = 0.0f;
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