//==============================================================================//
// Name : Shader.cpp															// 
// Describe :	シェーダ														// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/Texture.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLTexture.h"

namespace Fluoresce {

	Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height, const TexturetSpecification& option)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLTexture2D>(width, height, option);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, const TexturetSpecification& option)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLTexture2D>(path, option);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(uint32 width, uint32 height, const TexturetSpecification& option)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return  CreateRef<GLTextureCube>(width, height, option);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
	
	Ref<TextureCube> TextureCube::Create(const std::string& path, const TexturetSpecification& option)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLTextureCube>(path, option);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

}