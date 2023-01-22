//==============================================================================//
// Name : VertexBuffer.cpp														// 
// Describe :	頂点バッファ													// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/VertexBuffer.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLBuffer.h"

namespace Fluoresce {

	Ref<VertexBuffer> VertexBuffer::Create(uint32 size)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLVertexBuffer>(size);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float32* vertices, uint32 size)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLVertexBuffer>(vertices, size);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}