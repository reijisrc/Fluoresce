//==============================================================================//
// Name : VertexArray.h															// 
// Describe :	���_�z��														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/19														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"

namespace Fluoresce {

	// ���_�z��
	class VertexArray
	{
	public:
		enum class VertexStreamsType
		{
			Sequential = 0,
			Interleaved = 1,
		};

		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		virtual const VertexStreamsType GetVertexDataType() const = 0;

		static Ref<VertexArray> Create(VertexStreamsType type);
	};
}