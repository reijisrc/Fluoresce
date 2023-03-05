//==============================================================================//
// Name : GLUniformBuffer.cpp													// 
// Describe :	GLユニフォームバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLUniformBuffer.h"

#include <glad/glad.h>

namespace Fluoresce {

	GLUniformBuffer::GLUniformBuffer(uint32 size, uint32 binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		m_Size = size;
	}

	GLUniformBuffer::~GLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLUniformBuffer::SetData(const void* data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}