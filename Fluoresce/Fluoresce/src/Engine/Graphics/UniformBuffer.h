//==============================================================================//
// Name : UniformBuffer.h														// 
// Describe :	���j�t�H�[���o�b�t�@											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// ���j�t�H�[���o�b�t�@
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) = 0;

		virtual uint32 GetSize() const = 0;

		static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
	};
}