//==============================================================================//
// Name : KeyEvent.h															// 
// Describe : 	イベント:キー関連												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Input.h"

namespace Fluoresce {

	// キー入力イベント
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() { return m_KeyCode; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(KeyCode keycode) : m_KeyCode(keycode) { }
		KeyCode m_KeyCode;
	};

	// キープレスイベント
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, bool isRepeat = false) :
			KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	// キーリリースイベント
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	// キータイプイベント
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
};