//==============================================================================//
// Name : GLVertexArray.cpp														// 
// Describe :	GL’¸“_”z—ñ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/19														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLVertexArray.h"

#include <glad/glad.h>

namespace Fluoresce {

	static GLenum GPUDataTypeToOpenGLBaseType(GPUDataType type)
	{
		switch (type)
		{
		case GPUDataType::None:
			break;
		case GPUDataType::Float: return GL_FLOAT;
		case GPUDataType::Float2: return GL_FLOAT;
		case GPUDataType::Float3: return GL_FLOAT;
		case GPUDataType::Float4: return GL_FLOAT;
		case GPUDataType::Mat3: return GL_FLOAT;
		case GPUDataType::Mat4: return GL_FLOAT;
		case GPUDataType::Int: return GL_INT;
		case GPUDataType::Int2: return GL_INT;
		case GPUDataType::Int3: return GL_INT;
		case GPUDataType::Int4: return GL_INT;
		case GPUDataType::Bool: return GL_BOOL;
			break;
		default:
			break;
		}

		FR_CORE_ERROR("Unknown OpenGLBaseType!");
		return GL_FLOAT;
	}

	GLVertexArray::GLVertexArray(VertexStreamsType type) :
		m_Type(type)
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void GLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void GLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		FR_CORE_ASSERT(vertexBuffer->GetBufferLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetBufferLayout();
		uint32 index = 0;
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case GPUDataType::Float:
			case GPUDataType::Float2:
			case GPUDataType::Float3:
			case GPUDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					GetGPUDataTypeCount(element.Type),
					GPUDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					(m_Type == VertexStreamsType::Sequential) ? layout.GetStride() : element.Offset,
					(const void*)element.Offset);
				index++;
				break;
			}
			case GPUDataType::Int:
			case GPUDataType::Int2:
			case GPUDataType::Int3:
			case GPUDataType::Int4:
			case GPUDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					GetGPUDataTypeCount(element.Type),
					GPUDataTypeToOpenGLBaseType(element.Type),
					(m_Type == VertexStreamsType::Sequential) ? layout.GetStride() : element.Offset,
					(const void*)element.Offset);
				index++;
				break;
			}
			case GPUDataType::Mat3:
			case GPUDataType::Mat4:
			{
				uint8_t count = GetGPUDataTypeCount(element.Type);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						GPUDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						(m_Type == VertexStreamsType::Sequential) ? layout.GetStride() : element.Offset,
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				FR_CORE_ASSERT(false, "Unknown GPUDataType!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffers = indexBuffer;
	}
}