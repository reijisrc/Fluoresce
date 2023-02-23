//==============================================================================//
// Name : Window.h																// 
// Describe : 	モジュール:ウインドウ											// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2023/02/23														// 
//==============================================================================//
#pragma once

#include "frpch.h"

#include "Engine/Core/BaseDefine.h"
#include "Engine/Events/Event.h"

namespace Fluoresce {

	// ウインドウプロパティ
	struct WindowProps
	{
		std::string Title;
		uint32 Width;
		uint32 Height;

		WindowProps(const std::string& title,
			uint32_t width,
			uint32_t height)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	// ウインドウ
	class Window
	{
		using EventCallBackFn = std::function<void(Event&)>;

	public:
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void SetEventCallBack(const EventCallBackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual float32 GetWindowTime() const = 0;

		static Scope<Window> Create(const WindowProps& props);
	};
}