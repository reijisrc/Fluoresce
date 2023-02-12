//==============================================================================//
// Name : IndexBuffer.cpp														// 
// Describe :	インデックスバッファ											// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/IndexBuffer.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLBuffer.h"

namespace Fluoresce {

	Ref<IndexBuffer> IndexBuffer::Create(uint32* indices, uint32 count)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLIndexBuffer>(indices, count);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}