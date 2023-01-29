//==============================================================================//
// Name : RenderCommand.cpp														// 
// Describe :	レンダーコマンド												// 
// Author : Ding Qi																// 
// Create Date : 2022/08/15														// 
// Modify Date : 2022/08/15														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Fluoresce {

	Scope<GraphicsCore> RenderCommand::s_GraphicsCore = GraphicsCore::Create();
}
