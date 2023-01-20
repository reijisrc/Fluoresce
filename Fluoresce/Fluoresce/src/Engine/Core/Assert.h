//==============================================================================//
// Name : Assert.h																// 
// Describe :	アサート定義													// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/22														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Log.h"
#include <filesystem>

#ifdef FR_ENABLE_ASSERTS

#define FR_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { FR##type##ERROR(msg, __VA_ARGS__); FR_DEBUGBREAK(); } }
#define FR_INTERNAL_ASSERT_WITH_MSG(type, check, ...) FR_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define FR_INTERNAL_ASSERT_NO_MSG(type, check) FR_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", FR_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define FR_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define FR_INTERNAL_ASSERT_GET_MACRO(...) FR_EXPAND_MACRO( FR_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, FR_INTERNAL_ASSERT_WITH_MSG, FR_INTERNAL_ASSERT_NO_MSG) )

#define FR_ASSERT(...) FR_EXPAND_MACRO( FR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__) )
#define FR_CORE_ASSERT(...) FR_EXPAND_MACRO( FR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define FR_ASSERT(...)
#define FR_CORE_ASSERT(...)
#endif
