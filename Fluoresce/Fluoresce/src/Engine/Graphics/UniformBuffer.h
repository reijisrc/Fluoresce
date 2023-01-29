//==============================================================================//
// Name : UniformBuffer.h														// 
// Describe :	ユニフォームバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// ユニフォームバッファ
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
	};
}