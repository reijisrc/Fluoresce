//==============================================================================//
// Name : BatchRenderer.h														// 
// Describe :	�p�X�����_���[													// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// �o�b�`���v���
	struct BatchStatistics
	{
		uint32 DrawCalls = 0;
		uint32 VertexCount = 0;
		uint32 IndexCount = 0;
	};

	//	�o�b�`�����_���[
	class BatchRenderer
	{
	public:
		virtual ~BatchRenderer() = default;

		virtual void Submit() = 0;
		virtual void StartBatch() = 0;
		virtual void NextBatch() = 0;

		void ResetStats();

		BatchStatistics GetStats() const
		{
			return m_Stats;
		}
	protected:
		BatchStatistics m_Stats;
	};
};