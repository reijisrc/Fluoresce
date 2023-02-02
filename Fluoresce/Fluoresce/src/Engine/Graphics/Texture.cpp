//==============================================================================//
// Name : Shader.cpp															// 
// Describe :	シェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/Texture.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLTexture.h"

namespace Fluoresce {

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLTexture2D>(format, width, height);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLTexture2D>(path);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}