//==============================================================================//
// Name : Scene.cpp																// 
// Describe : 	ƒV[ƒ“															// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/SceneCamera.h"

namespace Fluoresce {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float32 size, float32 nearClip, float32 farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float32 verticalFOV, float32 nearClip, float32 farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32 width, uint32 height)
	{
		FR_CORE_ASSERT((width > 0 && height > 0), "Viewport Size = 0");
		m_AspectRatio = (float32)width / (float32)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float32 orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float32 orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float32 orthoBottom = -m_OrthographicSize * 0.5f;
			float32 orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}
}