//==============================================================================//
// Name : SpriteRenderer.cpp													// 
// Describe :	スプライトレンダラー											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/11														// 
// Modify Date : 2023/03/04														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/SkyboxRenderer.h"

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RenderPipeline.h"

namespace Fluoresce {

	struct SkyboxRenderer::RendererData
	{
		Ref<VertexArray>	VertexArray;
		Ref<VertexBuffer>	VertexBuffer;
		Ref<EffectShader>	Shader;
		Ref<TextureCube>	SkyboxTexture;
	};

	void SkyboxRenderer::Init(const std::string& shaderPath, const std::string& skyboxPath)
	{
		m_Data = new SkyboxRenderer::RendererData();
		m_Data->Shader = EffectShader::Create(shaderPath);
		m_Data->VertexArray = VertexArray::Create();

		// NDC基準のサイズ
		float32 fullScreenQuad[] =
		{
			-1.0f, -1.0f, 0.1f,
			1.0f, -1.0f, 0.1f,
			1.0f, 1.0f, 0.1f,
			-1.0f, 1.0f, 0.1f,
		};

		m_Data->VertexBuffer = VertexBuffer::Create(fullScreenQuad, 12 * sizeof(float32));
		m_Data->VertexBuffer->SetLayout({
			{ GPUDataType::Float3, "a_Postion"}
			});

		m_Data->VertexArray->AddVertexBuffer(m_Data->VertexBuffer);

		uint32 quadIndices[6] = { 0, 1, 2, 2, 3, 0, };

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, 6 * sizeof(uint32));
		m_Data->VertexArray->SetIndexBuffer(quadIB);

		m_Data->SkyboxTexture = TextureCube::Create(skyboxPath);
	}

	void SkyboxRenderer::ShutDown()
	{
		delete m_Data;
	}

	void SkyboxRenderer::Submit(const Camera& camera, const Mat4& transform)
	{
		Mat4 view = glm::inverse(transform);
		Mat4 proj = camera.GetProjection();
		Mat4 vp = proj * view;

		RenderPipeline::CameraData data;
		data.ViewProjection = vp;
		RenderPipeline::GetConstantBuffers().SetData<RenderPipeline::CameraData>(ConstantBuffer::ConstantBufferIndex::Camera, data);

		m_Data->Shader->Bind();
		m_Data->SkyboxTexture->Bind(0);
		RenderCommand::SetDepthTestFunc(DepthTestFunc::DepthTest_Lequal);
		RenderCommand::DrawIndexed(m_Data->VertexArray, 6);
		RenderCommand::SetDepthTestFunc(DepthTestFunc::DepthTest_Default);
	}

	void SkyboxRenderer::Submit(const EditorCamera& camera)
	{
		RenderPipeline::CameraData data;
		data.ViewProjection = camera.GetViewProjection();
		RenderPipeline::GetConstantBuffers().SetData<RenderPipeline::CameraData>(ConstantBuffer::ConstantBufferIndex::Camera, data);

		m_Data->Shader->Bind();
		m_Data->SkyboxTexture->Bind(0);
		RenderCommand::SetDepthTestFunc(DepthTestFunc::DepthTest_Lequal);
		RenderCommand::DrawIndexed(m_Data->VertexArray, 6);
		RenderCommand::SetDepthTestFunc(DepthTestFunc::DepthTest_Default);
	}
}