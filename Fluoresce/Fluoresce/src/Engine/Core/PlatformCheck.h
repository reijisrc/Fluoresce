// �v���b�h�t�H�[���ʂ̒�`
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
	#define FR_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* ��Ή��v���b�h�t�H�[�� */
	#error "Unknown platform!"
#endif
