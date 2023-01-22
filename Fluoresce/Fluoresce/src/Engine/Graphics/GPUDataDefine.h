//==============================================================================//
// Name : GPUDataDefine.h														// 
// Describe :	GPUデータ定義													// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	enum class GPUDataType
	{
		None = 0, 
		Float, 
		Float2, 
		Float3, 
		Float4, 
		Mat3, 
		Mat4, 
		Int, 
		Int2, 
		Int3, 
		Int4, 
		Bool
	};

	extern uint32 GetGPUDataTypeSize(GPUDataType type);
	extern uint32 GetGPUDataTypeCount(GPUDataType type);

	// バッファ要素
	struct BufferElements
	{
		std::string Name;
		GPUDataType Type;
		uint32 Size;
		size_t Offset;
		bool Normalized;

		BufferElements() = default;

		BufferElements(GPUDataType type, const std::string& name, bool normalized = false);
	};
}
