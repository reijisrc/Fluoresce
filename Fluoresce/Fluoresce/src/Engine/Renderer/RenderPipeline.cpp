//==============================================================================//
// Name : RenderPipeline.cpp													// 
// Describe : 	レンダーパイプライン											// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#include "frpch.h"

#include "Engine/Renderer/RenderPipeline.h"

namespace Fluoresce {

	struct RenderPipeline::SceneData
	{
		Mat4 viewProj;
	};

	RenderPipeline::SceneData* RenderPipeline::m_SceneData = new RenderPipeline::SceneData;

	void RenderPipeline::Init()
	{
		RenderCommand::Init();
	}

	void RenderPipeline::OnWindowResize(uint32 width, uint32 height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void RenderPipeline::BeginScene(const Mat4& transform, const Mat4& projection)
	{
		glm::mat4 view = glm::inverse(transform);
		m_SceneData->viewProj = projection * view;
	}

	void RenderPipeline::EndScene()
	{

	}
};

