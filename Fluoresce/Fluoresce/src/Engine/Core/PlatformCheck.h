// プラッドフォームチェック & プリプロセッサ
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
	/* Windows x64  */
	#define FR_PLATFORM_WINDOWS
	#else
	/* Windows x86 */
	#error "x86 Builds are not supported!"
	#endif"
#elif defined(__linux__)
	#error "Linux is not supported!"
#else
	/* 非対応プラッドフォーム */
	#error "Unknown platform!"
#endif
