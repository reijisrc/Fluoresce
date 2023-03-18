//==============================================================================//
// Name : GLBuffer.cpp															// 
// Describe :	GLバッファ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/12														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLBuffer.h"

#include <glad/glad.h>

namespace Fluoresce {

	GLVertexBuffer::GLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	GLVertexBuffer::GLVertexBuffer(const StandardVertexData& data)
	{
		bool tangentsExist = !data.Tangents.empty();

		const GLsizei positions_size = data.Positions.size() * sizeof(data.Positions[0]);
		const GLsizei normals_size = data.Normals.size() * sizeof(data.Normals[0]);
		const GLsizei texcoords_size = data.Texcoords.size() * sizeof(data.Texcoords[0]);
		const GLsizei tangents_size = tangentsExist ? data.Tangents.size() * sizeof(data.Tangents[0]) : 0;
		const GLsizei total_size = positions_size + normals_size + texcoords_size + tangents_size;

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferStorage(GL_ARRAY_BUFFER, m_RendererID, nullptr, GL_DYNAMIC_STORAGE_BIT);

		uint64 offset = 0;
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(positions_size), data.Positions.data());

		offset += positions_size;
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(normals_size), data.Normals.data());

		offset += normals_size;
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(texcoords_size), data.Texcoords.data());

		if (tangentsExist)
		{
			offset += texcoords_size;
			glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tangents_size), data.Tangents.data());

			SetLayout
			({ { GPUDataType::Float3, "a_Position" },
				{ GPUDataType::Float3, "a_Normal" },
				{ GPUDataType::Float2, "a_TexCoord" },
				{ GPUDataType::Float3, "a_Tangent" }
				});
		}
		else
		{
			SetLayout
			({ { GPUDataType::Float3, "a_Position" },
				{ GPUDataType::Float3, "a_Normal" },
				{ GPUDataType::Float2, "a_TexCoord" }
				});
		}
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void GLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::SetData(const void* data, uint32 offset, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	GLIndexBuffer::GLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void GLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}