//==============================================================================//
// Name : ScriptAutoRotation2D.cpp												// 
// Describe : 	スクリプト:自動回転												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#include "NativeScript/AutoRotation2D.h"

#include <glm/gtx/quaternion.hpp>

namespace Fluoresce {

	void ScriptAutoRotation2D::OnCreate()
	{
		FR_CLIENT_TRACE("ScriptAutoRotation2D Create");
	}

	void ScriptAutoRotation2D::OnDestroy()
	{
		FR_CLIENT_TRACE("ScriptAutoRotation2D Destroy");
	}

	void ScriptAutoRotation2D::OnUpdate(DeltaTime ts)
	{
		auto& translation = GetComponent<TransformComponent>().Translation;

		m_Rotate += (ts * m_AngularVelocity);
		glm::quat orientation = glm::quat(Vec3(0.0f, 0.0f, m_Rotate));
		Vec3 dir = glm::rotate(orientation, Vec3(0.0f, 1.0f, 0.0f));

		translation = dir * m_Radius;
	}
}
