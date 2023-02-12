//==============================================================================//
// Name : GLBuffer.h															// 
// Describe :	GLバッファ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/12														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"

namespace Fluoresce {

	// GL頂点バッファ
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(uint32_t size);
		GLVertexBuffer(float* vertices, uint32_t size);
		GLVertexBuffer(const StandardVertexData& data);

		virtual ~GLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const VertexBufferLayout& GetBufferLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID = 0;
		VertexBufferLayout m_Layout;
	};

	// GLインデックスバッファ
	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(uint32_t* vertices, uint32_t count);
		virtual ~GLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};

};
