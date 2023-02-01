//==============================================================================//
// Name : EditorCamera.cpp														// 
// Describe : 	エディターカメラ												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/07														// 
// Modify Date : 2023/01/08														// 
//==============================================================================//

#include "frpch.h"
#include "Engine/Graphics/EditorCamera.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Fluoresce {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(Mat4(1.0f))
	{
		Mat4 proj = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		SetProjection(proj);
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
		Vec3 position = CalculatePosition();
		glm::quat orientation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));

		m_ViewMatrix = glm::translate(Mat4(1.0f), position) * glm::toMat4(orientation);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}


	void EditorCamera::MousePan(const Vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
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
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	Vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float32, float32> EditorCamera::PanSpeed() const
	{
		float32 x = std::min(m_ViewportWidth / 1000.0f, 3.0f);
		float32 xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float32 y = std::min(m_ViewportHeight / 1000.0f, 3.0f);
		float32 yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float32 distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float32 speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}
