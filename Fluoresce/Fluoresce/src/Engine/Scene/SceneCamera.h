//==============================================================================//
// Name : SceneCamera.h															// 
// Describe : 	シーンカメラ													// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Camera.h"

namespace Fluoresce {

	// シーンカメラ
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float32 verticalFOV, float32 nearClip, float32 farClip);
		void SetOrthographic(float32 size, float32 nearClip, float32 farClip);

		void SetViewportSize(uint32 width, uint32 height);

		float32 GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float32 verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float32 GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float32 nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float32 GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float32 farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float32 GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float32 size) { m_OrthographicSize = size; RecalculateProjection(); }
		float32 GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float32 GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float32 m_PerspectiveFOV = Float::PI * 0.25f;
		float32 m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float32 m_OrthographicSize = 10.0f;
		float32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float32 m_AspectRatio = 0.0f;
	};
}