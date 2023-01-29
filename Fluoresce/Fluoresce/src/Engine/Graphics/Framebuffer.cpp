//==============================================================================//
// Name : Framebuffer.cpp														// 
// Describe : 	フレームバッファ												// 
// Author : Ding Qi																// 
// Create Date : 2022/10/15														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Graphics/Framebuffer.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLFramebuffer.h"

namespace Fluoresce {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: FR_CORE_ASSERT(false, "RenderAPI: None is currently not supported!") return nullptr;
		case GraphicsCore::API::OpenGL4: return CreateRef<GLFramebuffer>(spec);
		default:
			break;
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}
}