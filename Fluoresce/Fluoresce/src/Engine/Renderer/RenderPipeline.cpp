//==============================================================================//
// Name : RenderPipeline.cpp													// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/RenderPipeline.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Fluoresce {

	Ref<Texture2D> RenderPipeline::s_WhiteTexture = nullptr;
	Scope<ConstantBuffer> RenderPipeline::s_ConstantBuffers = CreateScope<ConstantBuffer>();
	Scope<LineRenderer> RenderPipeline::s_LineRenderer = CreateScope<LineRenderer>();
	Scope<SpriteRenderer> RenderPipeline::s_SpriteRenderer = CreateScope<SpriteRenderer>();
	Scope<SkyboxRenderer> RenderPipeline::s_SkyboxRenderer = CreateScope<SkyboxRenderer>();
	Scope<PostProcessingRenderer> RenderPipeline::s_PostProcessingRenderer = CreateScope<PostProcessingRenderer>();

	uint32 ConstantBuffer::GetMaxSize()
	{
		switch (GraphicsCore::GetAPI())
		{
		case GraphicsCore::API::None: return 0;
		case GraphicsCore::API::OpenGL4: return 16384;
		default:
			break;
		}
	}

	void ConstantBuffer::Init()
	{
		uint32 cameraData = sizeof(RenderPipeline::CameraData);
		uint32 environmentData = sizeof(RenderPipeline::HdrEnvironmentData);

		m_UniformBuffers.push_back(UniformBuffer::Create(cameraData, 0));
		m_UniformBuffers.push_back(UniformBuffer::Create(environmentData, 1));
	}

	void ConstantBuffer::ShutDown()
	{
		m_UniformBuffers.clear();
	}

	Ref<UniformBuffer> ConstantBuffer::GetBuffer(ConstantBufferIndex index)
	{
		uint32 idx = static_cast<uint32>(index);

		if (idx < ConstantBuffer::ConstantBufferIndex::Max)
		{
			return m_UniformBuffers.at(idx);
		}
		
		return nullptr;
	}

	void RenderPipeline::Init()
	{
		RenderCommand::Init();

		s_WhiteTexture = Texture2D::Create(1, 1);
		uint32 whiteData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteData, sizeof(whiteData));
		s_ConstantBuffers->Init();

		s_LineRenderer->Init("resources/shaders/Line.glsl");
		s_SpriteRenderer->Init("resources/shaders/Sprite.glsl");
		s_SkyboxRenderer->Init("resources/shaders/HDRskybox.glsl","resources/textures/DefaultCubeMap.hdr");

		//s_PostProcessingRenderer->Init("resources/shaders/ToneMapping.glsl");
		s_PostProcessingRenderer->Init("resources/shaders/ACESToneMapping.glsl");
	}

	void RenderPipeline::ShutDown()
	{
		s_LineRenderer->ShutDown();
		s_SpriteRenderer->ShutDown();
		s_SkyboxRenderer->ShutDown();
		s_PostProcessingRenderer->ShutDown();
		s_ConstantBuffers->ShutDown();
		s_WhiteTexture = nullptr;
	}

	void RenderPipeline::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void RenderPipeline::ResetAllBatchStats()
	{
		s_LineRenderer->ResetStats();
		s_SpriteRenderer->ResetStats();
	}

	void RenderPipeline::RequestClearAllBatchAssets()
	{
		s_SpriteRenderer->RequestClearTextureSlots();
	}

	Ref<Texture2D> RenderPipeline::GetWhiteTexture()
	{
		return s_WhiteTexture;
	}

	ConstantBuffer& RenderPipeline::GetConstantBuffers()
	{
		return *s_ConstantBuffers;
	}

	LineRenderer& RenderPipeline::GetLineRenderer()
	{
		return *s_LineRenderer;
	}

	SpriteRenderer& RenderPipeline::GetSpriteRenderer()
	{
		return *s_SpriteRenderer;
	}

	SkyboxRenderer& RenderPipeline::GetSkyboxRenderer()
	{
		return *s_SkyboxRenderer;
	}

	PostProcessingRenderer& RenderPipeline::GetPostProcessingRenderer()
	{
		return *s_PostProcessingRenderer;
	}

};

