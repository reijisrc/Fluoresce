//==============================================================================//
// Name : PassRenderer.cpp														// 
// Describe :	パスレンダラー													// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/BatchRenderer.h"

namespace Fluoresce {

	void BatchRenderer::ResetStats()
	{
		memset(&m_Stats, 0, sizeof(BatchStatistics));
	}
};