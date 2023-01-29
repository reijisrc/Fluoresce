//==============================================================================//
// Name : RenderPipeline.h														// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/LineRenderer.h"
#include "Engine/Renderer/SpriteRenderer.h"

namespace Fluoresce 
{

	//	レンダーパイプライン
	class RenderPipeline
	{
	public:
		enum class UniformBufferIndex
		{
			Camera = 0,
			Max,
		};

		struct CameraData
		{
			Mat4 ViewProjection;
		};

	public:
		static void Init();
		static void ShutDown();

		static void OnWindowResize(uint32 width, uint32 height);

		static Ref<UniformBuffer> GetUniformBuffer(UniformBufferIndex index);
		static LineRenderer& GetLineRenderer();
		static SpriteRenderer& GetSpriteRenderer();
	private:
		static std::vector<Ref<UniformBuffer>> s_UniformBuffers;
		static Scope<LineRenderer> s_LineRenderer;
		static Scope<SpriteRenderer> s_SpriteRenderer;
	};
}