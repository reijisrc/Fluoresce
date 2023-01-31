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

namespace Fluoresce {

	Mat4 TransformComponent::GetTransform() const
	{
		Mat4 mat = Mat4(1.0f);
		ComposeTransform(Translation, Rotation, Scale, mat);
		return mat;
	}

}