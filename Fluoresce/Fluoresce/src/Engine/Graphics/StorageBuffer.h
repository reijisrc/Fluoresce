//==============================================================================//
// Name : StorageBuffer.h														// 
// Describe :	�X�g���[�W�o�b�t�@												// 
// Author : Ding Qi																// 
// Create Date : 2023/03/05														// 
// Modify Date : 2023/03/05														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// ���j�t�H�[���o�b�t�@
	class StorageBuffer
	{
	public:
		virtual ~StorageBuffer() {}

		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) = 0;

		virtual uint32 GetSize() const = 0;

		static Ref<StorageBuffer> Create(uint32 size, uint32 binding);
	};
}