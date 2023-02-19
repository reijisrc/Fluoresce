//==============================================================================//
// Name : RenderPipeline.cpp													// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/RenderPipeline.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Fluoresce {

	Ref<Texture2D> RenderPipeline::s_WhiteTexture = nullptr;
	Scope<ConstBuffer> RenderPipeline::s_ConstBuffers = CreateScope<ConstBuffer>();
	Scope<LineRenderer> RenderPipeline::s_LineRenderer = CreateScope<LineRenderer>();
	Scope<SpriteRenderer> RenderPipeline::s_SpriteRenderer = CreateScope<SpriteRenderer>();
	Scope<SkyboxRenderer> RenderPipeline::s_SkyboxRenderer = CreateScope<SkyboxRenderer>();
	Scope<PostProcessingRenderer> RenderPipeline::s_PostProcessingRenderer = CreateScope<PostProcessingRenderer>();

	void ConstBuffer::Init()
	{
		uint32 cameraData = sizeof(RenderPipeline::CameraData);
		uint32 environmentData = sizeof(RenderPipeline::HdrEnvironmentData);

		m_UniformBuffers.push_back(UniformBuffer::Create(cameraData, 0));
		m_UniformBuffers.push_back(UniformBuffer::Create(environmentData, 1));

		m_Size += cameraData;
		m_Size += environmentData;
	}

	void ConstBuffer::ShutDown()
	{
		m_UniformBuffers.clear();
	}

	Ref<UniformBuffer> ConstBuffer::GetUniformBuffer(UniformBufferIndex index)
	{
		switch (index)
		{
		case Fluoresce::ConstBuffer::UniformBufferIndex::Camera:
			return m_UniformBuffers.at(0);
		case Fluoresce::ConstBuffer::UniformBufferIndex::HdrEnvironment:
			return m_UniformBuffers.at(1);
		case Fluoresce::ConstBuffer::UniformBufferIndex::Max:
			break;
		default:
			break;
		}

		return nullptr;
	}

	void RenderPipeline::Init()
	{
		RenderCommand::Init();

		s_WhiteTexture = Texture2D::Create(1, 1);
		uint32 whiteData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteData, sizeof(whiteData));
		s_ConstBuffers->Init();

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
		s_ConstBuffers->ShutDown();
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

	ConstBuffer& RenderPipeline::GetConstBuffers()
	{
		return *s_ConstBuffers;
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

