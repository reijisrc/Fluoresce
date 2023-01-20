//==============================================================================//
// Name : DataDefine.h															// 
// Describe :	データ定義														// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include <memory>

namespace Fluoresce {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	typedef signed char sint8;
	typedef unsigned char uint8;
	typedef int sint32;
	typedef unsigned int  uint32;
	typedef float float32;
	typedef long long sint64;
	typedef unsigned long long uint64;
}
