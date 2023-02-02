//==============================================================================//
// Name : BatchRenderer.cpp														// 
// Describe :	バッチレンダラー												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/BatchRenderer.h"

namespace Fluoresce {

	BatchBuffer BatchBuffer::Copy(void* data, uint32_t size)
	{
		BatchBuffer buffer;
		buffer.Allocate(size);
		memcpy(buffer.m_Data, data, size);
		return buffer;
	}

	void BatchBuffer::Allocate(uint32 size)
	{
		delete[] m_Data;
		m_Data = nullptr;

		if (size == 0)
			return;

		m_Data = new uint8[size];
		m_Size = size;
		m_ValidOffset = 0;
	}

	void BatchBuffer::Free()
	{
		if (m_Data)
		{
			delete[] m_Data;
			m_Data = nullptr;
		}
	}

	void BatchBuffer::Reset(bool resetData)
	{
		if (resetData)
		{
			if (m_Data)
			{
				memset(m_Data, 0, m_Size);
			}
		}
		m_ValidOffset = 0;
	}

	bool BatchBuffer::PushData(void* data, uint32 size)
	{
		if (m_ValidOffset + size <= m_Size)
		{
			memcpy(m_Data + m_ValidOffset, data, size);
			m_ValidOffset += size;
			return true;
		}
		return false;
	}

	void BatchRenderer::ResetBatchBuffer()
	{
		m_Buffer.Reset();
	}

	void BatchRenderer::ResetStats()
	{
		memset(&m_Stats, 0, sizeof(BatchStatistics));
	}
};