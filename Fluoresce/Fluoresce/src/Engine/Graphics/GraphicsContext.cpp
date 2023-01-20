//==============================================================================//
// Name : GraphicsContext.cpp													// 
// Describe : 	基底グラフィックスコンテキスト									// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/04/10														// 
//==============================================================================//
#include "frpch.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Engine/Graphics/GraphicsContext.h"
#include "Platform/OpenGL/GLContext.h"

namespace Fluoresce {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None:    FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!"); return nullptr;
		case GraphicsCore::API::OpenGL4:  return CreateScope<GLContext>(static_cast<GLFWwindow*>(window));
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

}