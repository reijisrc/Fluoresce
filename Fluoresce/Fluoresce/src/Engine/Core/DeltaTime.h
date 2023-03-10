//==============================================================================//
// Name : DeltaTime.h															// 
// Describe :	時間間隔														// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#pragma once
namespace Fluoresce {

	// 時間間隔
	class DeltaTime
	{
	public:
		DeltaTime(float time = 0.0f)
			: m_Time(time)
		{

		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliSeceons() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}