//==============================================================================//
// Name : Components.cpp														// 
// Describe : 	コンポーネント													// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/Components.h"
#include "Engine/Utils/GlmUtil.h"
#include "Engine/Asset/AssetsManager.h"

namespace Fluoresce {

	Mat4 TransformComponent::GetTransform() const
	{
		Mat4 mat = Mat4(1.0f);
		ComposeTransform(Translation, Rotation, Scale, mat);
		return mat;
	}

	bool SpriteRendererComponent::SetTextureAsset(const std::string& name)
	{
		auto& wTexture = AssetsManager::Get().GetTextureInstance(name);
		if (!wTexture.expired())
		{
			TextureName = name;
			Texture = wTexture;
			EnableTexture = true;
			return true;
		}

		FR_CORE_WARN("SpriteRendererComponent: Could not set texture {0}", name);
		return false;
	}

}