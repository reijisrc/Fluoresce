//==============================================================================//
// Name : GLShaderStorageBuffer.h												// 
// Describe :	GLストレージバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2023/03/05														// 
// Modify Date : 2023/03/05														// 
//==============================================================================//
#pragma once
#include "Engine/Graphics/StorageBuffer.h"

namespace Fluoresce {

	// GLストレージバッファ
	class GLShaderStorageBuffer : public StorageBuffer
	{
	public:
		GLShaderStorageBuffer(uint32 size, uint32 binding);
		virtual ~GLShaderStorageBuffer();

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) override;

		virtual uint32 GetSize() const override { return m_Size; }
	private:
		uint32 m_RendererID = 0;
		uint32 m_Size = 0;
	};

}
