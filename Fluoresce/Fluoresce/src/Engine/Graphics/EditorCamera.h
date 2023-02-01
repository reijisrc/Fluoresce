//==============================================================================//
// Name : EditorCamera.h														// 
// Describe : 	エディターカメラ												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/07														// 
// Modify Date : 2023/01/07														// 
//==============================================================================//
#pragma once

#include "Engine/Core/DeltaTime.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Graphics/Camera.h"

namespace Fluoresce {

	// エディターカメラ
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float32 fov, float32 aspectRatio, float32 nearClip, float32 farClip);

		void OnUpdate(DeltaTime ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
		Mat4 GetViewProjection() const;

		Vec3 GetUpDirection() const;
		Vec3 GetRightDirection() const;
		Vec3 GetForwardDirection() const;
		const Vec3& GetPosition() const { return m_Position; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const Vec2& delta);
		void MouseRotate(const Vec2& delta);
		void MouseZoom(float32 delta);

		Vec3 CalculatePosition() const;

		std::pair<float32, float32> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float32 m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		Mat4 m_ViewMatrix;
		Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		Vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		Vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float32 m_Distance = 10.0f;
		float32 m_Pitch = 0.0f, m_Yaw = 0.0f;

		float32 m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}