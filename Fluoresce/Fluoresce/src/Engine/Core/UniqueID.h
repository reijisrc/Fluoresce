//==============================================================================//
// Name : UniqueID.h															// 
// Describe : 	グローバルユニークID											// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#pragma once

namespace Fluoresce
{
	class UniqueID
	{
	public:
		UniqueID();
		UniqueID(uint64 uid);
		UniqueID(const UniqueID&) = default;

		operator uint64() const { return m_UID; }
	private:
		uint64 m_UID;
	};
}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<Fluoresce::UniqueID>
	{
		std::size_t operator()(const Fluoresce::UniqueID& uid) const
		{
			return (uint64_t)uid;
		}
	};

}