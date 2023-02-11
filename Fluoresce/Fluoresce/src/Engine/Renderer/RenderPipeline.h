//==============================================================================//
// Name : RenderPipeline.h														// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/LineRenderer.h"
#include "Engine/Renderer/SpriteRenderer.h"
#include "Engine/Renderer/SkyboxRenderer.h"
#include "Engine/Renderer/PostProcessingRenderer.h"

namespace Fluoresce 
{
	//	レンダーパイプライン
	class RenderPipeline
	{
	public:
		enum class UniformBufferIndex
		{
			Camera = 0,
			HdrEnvironment = 1,
			Max,
		};

		struct CameraData
		{
			Mat4 ViewProjection;
		};

		struct HdrEnvironmentData
		{
			float32 Gamma;
			float32 Exposure;
		};

	public:
		static void Init();
		static void ShutDown();

		static void OnWindowResize(uint32 width, uint32 height);

		static void ResetAllBatchStats();
		static void RequestClearAllBatchAssets();

		static Ref<Texture2D> GetWhiteTexture();
		static Ref<UniformBuffer> GetUniformBuffer(UniformBufferIndex index);
		static LineRenderer& GetLineRenderer();
		static SpriteRenderer& GetSpriteRenderer();
		static SkyboxRenderer& GetSkyboxRenderer();
		static PostProcessingRenderer& GetPostProcessingRenderer();
	private:
		static Ref<Texture2D> s_WhiteTexture;
		static std::vector<Ref<UniformBuffer>> s_UniformBuffers;
		static Scope<LineRenderer> s_LineRenderer;
		static Scope<SpriteRenderer> s_SpriteRenderer;
		static Scope<SkyboxRenderer> s_SkyboxRenderer;
		static Scope<PostProcessingRenderer> s_PostProcessingRenderer;
	};
}