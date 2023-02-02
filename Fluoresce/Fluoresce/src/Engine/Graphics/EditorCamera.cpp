//==============================================================================//
// Name : EditorCamera.cpp														// 
// Describe : 	エディターカメラ												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/07														// 
// Modify Date : 2023/01/22														// 
//==============================================================================//

#include "frpch.h"
#include "Engine/Graphics/EditorCamera.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Fluoresce {

	EditorCamera::EditorCamera(float32 fov, float32 aspectRatio, float32 nearClip, float32 farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(DeltaTime ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			Vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FR_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	void EditorCamera::SetFocusPoint(const Vec3& focuspoint, bool recalculateView)
	{
		m_FocusPoint = focuspoint;
		if (recalculateView)
		{
			UpdateView();
		}
	}

	void EditorCamera::ResetView(float32 aspect)
	{
		m_AspectRatio = aspect;
		m_FOV = 45.0f;
		m_NearClip = 0.1f;
		m_FarClip = 1000.0f;
		m_Position = { 0.0f, 0.0f, 0.0f };
		m_FocusPoint = { 0.0f, 0.0f, 0.0f };
		m_InitialMousePosition = { 0.0f, 0.0f };
		m_Distance = 10.0f;
		m_Pitch = 0.0f;
		m_Yaw = 0.0f;
		m_RotationSpeed = 0.8f;
		UpdateView();
	}

	Mat4 EditorCamera::GetViewProjection() const
	{
		return m_Projection * m_ViewMatrix;
	}

	Vec3 EditorCamera::GetUpDirection() const
	{
		glm::quat orientation = glm::quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
		return glm::rotate(orientation, Vec3(0.0f, 1.0f, 0.0f));
	}

	Vec3 EditorCamera::GetRightDirection() const
	{
		glm::quat orientation = glm::quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
		return glm::rotate(orientation, Vec3(1.0f, 0.0f, 0.0f));
	}

	Vec3 EditorCamera::GetForwardDirection() const
	{
		glm::quat orientation = glm::quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
		return glm::rotate(orientation, Vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();
		glm::quat orientation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));

		m_ViewMatrix = glm::translate(Mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float32 delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const Vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocusPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocusPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const Vec2& delta)
	{
		float32 yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float32 delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocusPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	Vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocusPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float32, float32> EditorCamera::PanSpeed() const
	{
		float32 x = std::min(m_ViewportWidth / 1000.0f, 3.0f);
		float32 xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float32 y = std::min(m_ViewportHeight / 1000.0f, 3.0f);
		float32 yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float32 EditorCamera::ZoomSpeed() const
	{
		float32 distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float32 speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}
