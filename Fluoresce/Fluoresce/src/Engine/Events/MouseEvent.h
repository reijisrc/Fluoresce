//==============================================================================//
// Name : MouseEvent.h															// 
// Describe : 	�C�x���g:�}�E�X�֘A												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Input.h"

namespace Fluoresce {

	// �}�E�X�ړ��C�x���g
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float32 x, float32 y) :
			m_MouseX(x), m_MouseY(y) {};

		float32 GetX() const { return m_MouseX; };
		float32 GetY() const { return m_MouseY; };

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent:" << m_MouseX << " , " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float32 m_MouseX, m_MouseY;
	};

	// �}�E�X�X�N���[���C�x���g
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float32 xOffset, float32 yOffset) :
			m_XOffset(xOffset), m_YOffset(yOffset) {};

		float32 GetXOffset() const { return m_XOffset; };
		float32 GetYOffset() const { return m_YOffset; };

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << GetXOffset() << " , " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float32 m_XOffset, m_YOffset;
	};

	// �}�E�X���̓C�x���g
	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(MouseCode button)
			: m_Button(button) { }

		MouseCode m_Button;
	};

	// �}�E�X�v���X�C�x���g
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button) :
			MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// �}�E�X�����[�X�C�x���g
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button) :
			MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent:" << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
};