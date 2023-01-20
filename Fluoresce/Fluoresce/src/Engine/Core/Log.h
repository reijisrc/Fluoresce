//==============================================================================//
// Name : Log.h																	// 
// Describe :	モジュール:ログ													// 
// Author : Ding Qi																// 
// Create Date : 2022/03/18														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/DataDefine.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Fluoresce {

	// ログ
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// エンジン側 ログ出力マクロ
#define FR_CORE_TRACE(...)    ::Fluoresce::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FR_CORE_INFO(...)     ::Fluoresce::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FR_CORE_WARN(...)     ::Fluoresce::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FR_CORE_ERROR(...)    ::Fluoresce::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FR_CORE_CRITICAL(...) ::Fluoresce::Log::GetCoreLogger()->critical(__VA_ARGS__)

// クライアント側 ログ出力マクロ
#define FR_CLIENT_TRACE(...)         ::Fluoresce::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FR_CLIENT_INFO(...)          ::Fluoresce::Log::GetClientLogger()->info(__VA_ARGS__)
#define FR_CLIENT_WARN(...)          ::Fluoresce::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FR_CLIENT_ERROR(...)         ::Fluoresce::Log::GetClientLogger()->error(__VA_ARGS__)
#define FR_CLIENT_CRITICAL(...)      ::Fluoresce::Log::GetClientLogger()->critical(__VA_ARGS__)
