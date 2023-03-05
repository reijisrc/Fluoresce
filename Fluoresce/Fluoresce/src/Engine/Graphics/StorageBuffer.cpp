//==============================================================================//
// Name : StorageBuffer.h														// 
// Describe :	ストレージバッファ												// 
// Author : Ding Qi																// 
// Create Date : 2023/03/05														// 
// Modify Date : 2023/03/05														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/StorageBuffer.h"
#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLShaderStorageBuffer.h"

namespace Fluoresce {

	Ref<StorageBuffer> StorageBuffer::Create(uint32 size, uint32 binding)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLShaderStorageBuffer>(size, binding);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}