//==============================================================================//
// Name : PostProcessingRenderer.h												// 
// Describe :	ポストプロセスレンダラー										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/11														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/EditorCamera.h"

#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Framebuffer.h"

namespace Fluoresce {

	//	ポストプロセスレンダラー
	class PostProcessingRenderer
	{
	public:
		void Init(const std::string& shaderPath);
		void ShutDown();

		void Submit(const Ref<Framebuffer>& framebuffer, float32 exposure);
	private:
		struct RendererData;
		RendererData* m_Data;
	};
};