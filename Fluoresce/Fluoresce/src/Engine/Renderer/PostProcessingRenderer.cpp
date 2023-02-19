//==============================================================================//
// Name : PostProcessingRenderer.cpp											// 
// Describe :	ポストプロセスレンダラー										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/11														// 
// Modify Date : 2023/02/20														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Renderer/PostProcessingRenderer.h"

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Graphics/UniformBuffer.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/RenderPipeline.h"

#include <glad/glad.h>

namespace Fluoresce {

	constexpr float32 gamma = 2.2f;

	struct PostProcessingRenderer::RendererData
	{
		Ref<VertexArray>	VertexArray;
		Ref<VertexBuffer>	VertexBuffer;
		Ref<EffectShader>			ToneMappingShader;
	};

	void PostProcessingRenderer::Init(const std::string& shaderPath)
	{
		m_Data = new PostProcessingRenderer::RendererData();
		m_Data->ToneMappingShader = EffectShader::Create(shaderPath);
		m_Data->VertexArray = VertexArray::Create();

		float32 fullScreenQuad[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		m_Data->VertexBuffer = VertexBuffer::Create(fullScreenQuad, 20 * sizeof(float32));
		m_Data->VertexBuffer->SetLayout({
			{ GPUDataType::Float3, "a_Postion"},
			{ GPUDataType::Float2, "a_TexCoord"}
			});

		m_Data->VertexArray->AddVertexBuffer(m_Data->VertexBuffer);

		uint32 quadIndices[6] = { 0, 1, 2, 2, 3, 0, };

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, 6 * sizeof(uint32));
		m_Data->VertexArray->SetIndexBuffer(quadIB);
	}

	void PostProcessingRenderer::ShutDown()
	{
		delete m_Data;
	}

	void PostProcessingRenderer::Submit(const Ref<Framebuffer>& framebuffer, float32 exposure)
	{
		if (auto ubo = RenderPipeline::GetConstBuffers().GetUniformBuffer(ConstBuffer::UniformBufferIndex::HdrEnvironment); ubo)
		{
			RenderPipeline::HdrEnvironmentData environment;
			environment.Gamma = gamma;
			environment.Exposure = exposure;
			ubo->SetData(&environment, sizeof(RenderPipeline::HdrEnvironmentData));
		}

		m_Data->ToneMappingShader->Bind();
		framebuffer->BindAttachmentToTextureSlot(0);
		RenderCommand::DrawIndexed(m_Data->VertexArray, 6);
	}
}
