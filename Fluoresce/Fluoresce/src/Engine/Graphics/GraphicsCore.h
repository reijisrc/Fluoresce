//==============================================================================//
// Name : GraphicsCore.h														// 
// Describe : 	グラフィックスコア												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// グラフィックコア
	class GraphicsCore
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL4 = 1,
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		virtual void Clear() = 0;

		virtual void SetLineWidth(float32 width) = 0;

		inline static API GetAPI() { return s_API; }

		static Scope<GraphicsCore> Create();
	private:
		static API s_API;
	};
}