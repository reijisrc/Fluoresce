//==============================================================================//
// Name : RenderPipeline.cpp													// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/RenderPipeline.h"
#include "Engine/Renderer/RenderCommand.h"

namespace Fluoresce {

	std::vector<Ref<UniformBuffer>> RenderPipeline::s_UniformBuffers;
	Scope<LineRenderer> RenderPipeline::s_LineRenderer = CreateScope<LineRenderer>();
	Scope<SpriteRenderer> RenderPipeline::s_SpriteRenderer = CreateScope<SpriteRenderer>();

	void RenderPipeline::Init()
	{
		RenderCommand::Init();
		s_UniformBuffers.push_back(UniformBuffer::Create(sizeof(CameraData), 0));
		s_LineRenderer->Init("resources/shaders/Line.glsl");
		s_SpriteRenderer->Init("resources/shaders/Sprite.glsl");
	}

	void RenderPipeline::ShutDown()
	{
		s_LineRenderer->ShutDown();
		s_SpriteRenderer->ShutDown();
	}

	void RenderPipeline::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
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

