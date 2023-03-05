//==============================================================================//
// Name : GLShaderStorageBuffer.CPP												// 
// Describe :	GLストレージバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2023/03/05														// 
// Modify Date : 2023/03/05														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/OpenGL/GLShaderStorageBuffer.h"

#include <glad/glad.h>

namespace Fluoresce {

	GLShaderStorageBuffer::GLShaderStorageBuffer(uint32 size, uint32 binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		m_Size = size;
	}

	GLShaderStorageBuffer::~GLShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLShaderStorageBuffer::SetData(const void* data, uint32 size, uint32 offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}