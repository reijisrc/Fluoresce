//==============================================================================//
// Name : GLUniformBuffer.h														// 
// Describe :	GL���j�t�H�[���o�b�t�@											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once
#include "Engine/Graphics/UniformBuffer.h"

namespace Fluoresce {

	// GL���j�t�H�[���o�b�t�@
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer(uint32 size, uint32 binding);
		virtual ~GLUniformBuffer();

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) override;
	private:
		uint32 m_RendererID = 0;
	};
}