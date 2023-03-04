//==============================================================================//
// Name : RenderPipeline.h														// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2023/03/04														// 
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
	class ConstantBuffer
	{
	public:
		enum ConstantBufferIndex : uint32 
		{
			Camera = 0,
			HdrEnvironment = 1,
			Max,
		};

		static uint32 GetMaxSize();

	public:
		void Init();
		void ShutDown();

		template<typename T>
		void SetData(ConstantBufferIndex index, T& data)
		{
			uint32 idx = static_cast<uint32>(index);

			if (idx < ConstantBuffer::ConstantBufferIndex::Max)
			{
				uint32 size = 0;
				for (uint32 i = 0; i < idx; i++)
				{
					size += m_UniformBuffers.at(i)->GetSize();
				}

				uint32 datasize = sizeof(T);
				if (size + datasize < GetMaxSize())
				{
					m_UniformBuffers.at(idx)->SetData(&data, datasize);
				}
			}
		}

		Ref<UniformBuffer> GetBuffer(ConstantBufferIndex index);
	private:
		std::vector<Ref<UniformBuffer>> m_UniformBuffers;
	};

	//	レンダーパイプライン
	class RenderPipeline
	{
	public:
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
		static ConstantBuffer& GetConstantBuffers();
		static LineRenderer& GetLineRenderer();
		static SpriteRenderer& GetSpriteRenderer();
		static SkyboxRenderer& GetSkyboxRenderer();
		static PostProcessingRenderer& GetPostProcessingRenderer();
	private:
		static Ref<Texture2D> s_WhiteTexture;
		static Scope<ConstantBuffer> s_ConstantBuffers;
		static Scope<LineRenderer> s_LineRenderer;
		static Scope<SpriteRenderer> s_SpriteRenderer;
		static Scope<SkyboxRenderer> s_SkyboxRenderer;
		static Scope<PostProcessingRenderer> s_PostProcessingRenderer;
	};
}