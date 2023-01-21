//==============================================================================//
// Name : RenderPipeline.h														// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/GraphicsCore.h"

namespace Fluoresce 
{
	//	レンダーパイプライン
	class RenderPipeline
	{
	public:
		static void Init();
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(const Mat4& transform, const Mat4& projection);
		static void EndScene();

		static GraphicsCore::API GetAPI() { return GraphicsCore::GetAPI(); }

	private:
		struct SceneData;
		static SceneData* m_SceneData;
	};
}