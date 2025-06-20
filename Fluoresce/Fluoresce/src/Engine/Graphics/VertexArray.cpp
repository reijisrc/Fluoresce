//==============================================================================//
// Name : VertexArray.cpp														// 
// Describe :	���_�z��														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/VertexArray.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLVertexArray.h"

namespace Fluoresce {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLVertexArray>();
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}