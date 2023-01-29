//==============================================================================//
// Name : GraphicsCore.cpp														// 
// Describe : 	グラフィックスコア												// 
// Author : Ding Qi																// 
// Create Date : 2022/04/10														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#include "frpch.h"

#include "Engine/Graphics/GraphicsCore.h"
#include "Platform/OpenGL/GLCore.h"

namespace Fluoresce {

	Scope<GraphicsCore> GraphicsCore::Create()
	{
		switch (s_API)
		{
		case GraphicsCore::API::None:    FR_CORE_ASSERT(false, "GraphicsCore::API::None is currently not supported!"); return nullptr;
		case GraphicsCore::API::OpenGL4:  return CreateScope<GLCore>();
		}

		FR_CORE_ASSERT(false, "Unknown GraphicsAPI!");
		return nullptr;
	}

	// OpenGL4固定
	GraphicsCore::API GraphicsCore::s_API = GraphicsCore::API::OpenGL4;
}