//==============================================================================//
// Name : BatchRenderer.h														// 
// Describe :	バッチレンダラー												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// バッチ統計情報
	struct BatchStatistics
	{
		uint32 DrawCalls = 0;
		uint32 VertexCount = 0;
		uint32 IndexCount = 0;
	};

	// バッチバッファ
	class BatchBuffer
	{
	public:
		BatchBuffer()
			: m_Data(nullptr), m_Size(0), m_ValidOffset(0)
		{
		}

		BatchBuffer(uint8* data, uint32 size)
			: m_Data(data), m_Size(size), m_ValidOffset(0)
		{
		}

		static BatchBuffer Copy(void* data, uint32_t size);

		void Allocate(uint32 size);

		void Free();

		void Reset(bool resetData = true);

		bool PushData(void* data, uint32 size);

		operator bool() const
		{
			return m_Data;
		}

		uint8& operator[](int index)
		{
			return m_Data[index];
		}

		uint8 operator[](int index) const
		{
			return m_Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)m_Data;
		}

		uint32 GetSize() const { return m_Size; }
		uint32 GetValidOffset() const { return m_ValidOffset; }
	private:
		uint8* m_Data;
		uint32 m_Size;
		uint32 m_ValidOffset;
	};

	//	バッチレンダラー
	class BatchRenderer
	{
	public:
		virtual ~BatchRenderer() = default;

		virtual void Submit() = 0;
		virtual void StartBatch() = 0;
		virtual void NextBatch() = 0;
		
		void ResetBatchBuffer();
		void ResetStats();

		BatchStatistics GetStats() const
		{
			return m_Stats;
		}
	protected:
		BatchBuffer		m_Buffer;
		BatchStatistics m_Stats;
	};
};