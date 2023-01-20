//==============================================================================//
// Name : ApplicationEvent.h													// 
// Describe : 	�C�x���g:�A�v���P�[�V����										// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Events/Event.h"

namespace Fluoresce {

	// �E�C���h�E���T�C�Y�C�x���g
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height) :
			m_Width(width), m_Height(height) {};

		uint32 GetWidth() { return m_Width; };
		uint32 GetHeight() { return m_Height; };

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowRectEvent:" << m_Width << " , " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32 m_Width, m_Height;
	};

	// �E�B���h�E����C�x���g
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// �A�v���P�[�V�����Œ�Ԋu���s�C�x���g
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// �A�v���P�[�V�����X�V�C�x���g
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// �A�v���P�[�V�����`��C�x���g
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
};