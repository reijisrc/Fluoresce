//==============================================================================//
// Name : GLVertexArray.h														// 
// Describe :	GL頂点配列														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/19														// 
//==============================================================================//
#pragma once
#include "Engine/Graphics/VertexArray.h"

namespace Fluoresce {

	// GL頂点配列
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray(VertexStreamsType type);
		virtual ~GLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffers; }

		virtual const VertexStreamsType GetVertexDataType() const { return m_Type; }
	private:
		uint32_t m_RendererID = 0;
		VertexStreamsType m_Type;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;
	};
}