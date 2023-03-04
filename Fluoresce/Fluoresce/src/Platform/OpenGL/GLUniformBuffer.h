//==============================================================================//
// Name : GLUniformBuffer.h														// 
// Describe :	GLユニフォームバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#pragma once
#include "Engine/Graphics/UniformBuffer.h"

namespace Fluoresce {

	// GLユニフォームバッファ
	class GLUniformBuffer : public UniformBuffer
	{
	public:
		GLUniformBuffer(uint32 size, uint32 binding);
		virtual ~GLUniformBuffer();

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) override;

		virtual uint32 GetSize() const override { return m_Size; }
	private:
		uint32 m_RendererID = 0;
		uint32 m_Size = 0;
	};
}