//==============================================================================//
// Name : VertexBuffer.h														// 
// Describe :	頂点バッファ													// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/12														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GPUDataDefine.h"
#include "Engine/Graphics/Mesh.h"

namespace Fluoresce {

	// 頂点バッファレイアウト
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() { }
		VertexBufferLayout(const std::initializer_list<BufferElements>& elements)
			: m_Elements(elements), m_Stride(0)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32 GetStride() const { return m_Stride; }
		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElements>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); }
	private:

		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			for (auto& element : m_Elements)
			{
				element.Offset += offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElements> m_Elements;
		uint32 m_Stride;
	};

	// 頂点バッファ
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32 offset, uint32_t size) = 0;

		virtual const VertexBufferLayout& GetBufferLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32 size);
		static Ref<VertexBuffer> Create(float32* vertices, uint32 size);
		static Ref<VertexBuffer> Create(const StandardVertexData& data);
	};

};