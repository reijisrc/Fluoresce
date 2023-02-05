//==============================================================================//
// Name : RenderPipeline.cpp													// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/RenderPipeline.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Fluoresce {

	Ref<Texture2D> RenderPipeline::s_WhiteTexture = nullptr;
	std::vector<Ref<UniformBuffer>> RenderPipeline::s_UniformBuffers;
	Scope<LineRenderer> RenderPipeline::s_LineRenderer = CreateScope<LineRenderer>();
	Scope<SpriteRenderer> RenderPipeline::s_SpriteRenderer = CreateScope<SpriteRenderer>();

	void RenderPipeline::Init()
	{
		RenderCommand::Init();

		s_WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1);
		uint32 whiteData = 0xffffffff;
		s_WhiteTexture->SetData(&whiteData, sizeof(whiteData));
		s_UniformBuffers.push_back(UniformBuffer::Create(sizeof(CameraData), 0));
		s_LineRenderer->Init("resources/shaders/Line.glsl");
		s_SpriteRenderer->Init("resources/shaders/Sprite.glsl");
	}

	void RenderPipeline::ShutDown()
	{
		s_LineRenderer->ShutDown();
		s_SpriteRenderer->ShutDown();
		s_UniformBuffers.clear();
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

	Ref<UniformBuffer> RenderPipeline::GetUniformBuffer(RenderPipeline::UniformBufferIndex index)
	{
		switch (index)
		{
		case Fluoresce::RenderPipeline::UniformBufferIndex::Camera:
			return s_UniformBuffers.at(0);
		case Fluoresce::RenderPipeline::UniformBufferIndex::Max:
			break;
		default:
			break;
		}

		return nullptr;
	}

	LineRenderer& RenderPipeline::GetLineRenderer()
	{
		return *s_LineRenderer;
	}

	SpriteRenderer& RenderPipeline::GetSpriteRenderer()
	{
		return *s_SpriteRenderer;
	}

};

