//==============================================================================//
// Name : BaseDefine.h															// 
// Describe :	É}ÉNÉçíËã`														// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/PlatformCheck.h"

#include <memory>

#ifdef FR_DEBUG
	#if defined(FR_PLATFORM_WINDOWS)
		#define FR_DEBUGBREAK() __debugbreak()	
	#elif defined(FR_PLATFORM_LINUX)
		#include <signal.h>
		#define FR_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define FR_ENABLE_ASSERTS
#else
	#define FR_DEBUGBREAK()
#endif

#define FR_EXPAND_MACRO(x) x
#define FR_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define FR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 