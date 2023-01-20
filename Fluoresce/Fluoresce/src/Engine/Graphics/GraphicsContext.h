//==============================================================================//
// Name : GraphicsContext.h														// 
// Describe : 	���O���t�B�b�N�X�R���e�L�X�g									// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// ���O���t�B�b�N�X�R���e�L�X�g
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static Scope<GraphicsContext> Create(void* window);
	private:
	};
}