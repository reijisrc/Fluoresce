//==============================================================================//
// Name : IndexBuffer.h															// 
// Describe :	�C���f�b�N�X�o�b�t�@											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

#include "GPUDataDefine.h"

namespace Fluoresce {

	// �C���f�b�N�X�o�b�t�@
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32* indices, uint32 count);
	};
}