//==============================================================================//
// Name : GraphicsContext.h														// 
// Describe : 	基底グラフィックスコンテキスト									// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// 基底グラフィックスコンテキスト
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static Scope<GraphicsContext> Create(void* window);
	private:
	};
}