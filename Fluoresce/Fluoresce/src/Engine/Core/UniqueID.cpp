//==============================================================================//
// Name : UniqueID.cpp															// 
// Describe : 	グローバルユニークID											// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#include "frpch.h"
#include "UniqueID.h"

#include <random>

namespace Fluoresce {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64> s_UniformDistribution;

	UniqueID::UniqueID()
		: m_UID(s_UniformDistribution(s_Engine))
	{
	}

	UniqueID::UniqueID(uint64 uid)
		: m_UID(uid)
	{
	}

}