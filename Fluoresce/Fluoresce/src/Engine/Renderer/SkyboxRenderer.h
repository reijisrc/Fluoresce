//==============================================================================//
// Name : SpriteRenderer.h														// 
// Describe :	�X�v���C�g�����_���[											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/11														// 
// Modify Date : 2023/02/11														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/EditorCamera.h"

#include "Engine/Graphics/Texture.h"

namespace Fluoresce {

	//	�X�K�C�{�b�N�X�����_���[
	class SkyboxRenderer
	{
	public:
		void Init(const std::string& shaderPath, const std::string& skyboxPath);
		void ShutDown();

		void Submit(const Camera& camera, const Mat4& transform);
		void Submit(const EditorCamera& camera);
	private:
		struct RendererData;
		RendererData* m_Data;
	};
};