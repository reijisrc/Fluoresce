//==============================================================================//
// Name : FileUtil.h															// 
// Describe :	�t�@�C���ėp�@�\												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/08														// 
// Modify Date : 2023/01/09														// 
//==============================================================================//
#pragma once

#include <string>

namespace Fluoresce {

	// �t�@�C���ėp�@�\
	class FileUtil
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}