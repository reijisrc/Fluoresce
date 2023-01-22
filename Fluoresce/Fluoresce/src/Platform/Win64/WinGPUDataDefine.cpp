//==============================================================================//
// Name : WinGPUDataDefine.cpp													// 
// Describe : 	Win(x64)GPUデータ定義											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#include "frpch.h"

#include "Engine/Graphics/GPUDataDefine.h"

namespace Fluoresce
{
	uint32 GetGPUDataTypeSize(GPUDataType type)
	{
		switch (type)
		{
		case GPUDataType::None:
			break;
		case GPUDataType::Float: return 4;
		case GPUDataType::Float2: return 4 * 2;
		case GPUDataType::Float3: return 4 * 3;
		case GPUDataType::Float4: return 4 * 4;
		case GPUDataType::Mat3: return 4 * 3 * 3;
		case GPUDataType::Mat4: return 4 * 4 * 4;
		case GPUDataType::Int: return 4;
		case GPUDataType::Int2: return 4 * 2;
		case GPUDataType::Int3: return 4 * 3;
		case GPUDataType::Int4: return 4 * 4;
		case GPUDataType::Bool: return 1;
			break;
		default:
			break;
		}

		FR_CORE_ERROR("Unknown GPUDataType!");
		return 0;
	}

	uint32 GetGPUDataTypeCount(GPUDataType type)
	{
		switch (type)
		{
		case GPUDataType::None:
			break;
		case GPUDataType::Float: return 1;
		case GPUDataType::Float2: return 2;
		case GPUDataType::Float3: return 3;
		case GPUDataType::Float4: return 4;
		case GPUDataType::Mat3: return 3 * 3;
		case GPUDataType::Mat4: return 4 * 4;
		case GPUDataType::Int: return 1;
		case GPUDataType::Int2: return 2;
		case GPUDataType::Int3: return 3;
		case GPUDataType::Int4: return 4;
		case GPUDataType::Bool: return 1;
			break;
		default:
			break;
		}

		FR_CORE_ERROR("Unknown ShaderDataType!");
		return 0;
	}

	BufferElements::BufferElements(GPUDataType type, const std::string& name, bool normalized)
		: Name(name), Type(type), Size(GetGPUDataTypeCount(type)), Offset(0), Normalized(normalized)
	{

	}
}