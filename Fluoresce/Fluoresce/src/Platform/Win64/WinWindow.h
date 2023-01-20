//==============================================================================//
// Name : WinWindow.h															// 
// Describe : 	Win(x64)ウインドウ												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Graphics/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Fluoresce {

	// Win(x64)ウインドウ
	class WinWindow : public Window
	{
		using EventCallBackFn = std::function<void(Event&)>;

	public:
		WinWindow(const WindowProps& props);
		virtual ~WinWindow();

		virtual void OnUpdate() override;

		unsigned int GetWidth() const override { return  m_Data.Width; }
		unsigned int GetHeight() const override { return  m_Data.Height; }

		void SetEventCallBack(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const override { return m_Window; };

		virtual float32 GetWindowTime() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallBackFn EventCallBack;
		};

		WindowData m_Data;
	};
}