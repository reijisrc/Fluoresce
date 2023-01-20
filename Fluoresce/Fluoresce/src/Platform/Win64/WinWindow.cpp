//==============================================================================//
// Name : WinWindow.cpp															// 
// Describe : 	Win(x64)ウインドウ												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#include "frpch.h"
#include "Platform/Win64/WinWindow.h"

#include "Engine/Core/Input.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Fluoresce {

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallBack(int error, const char* description)
	{
		FR_CORE_ERROR("GLFW Error ({0}): {1})", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WinWindow>(props);
	}

	WinWindow::WinWindow(const WindowProps& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		ShutDown();
	}

	void WinWindow::OnUpdate()
	{
		m_Context->SwapBuffer();
		glfwPollEvents();
	}

	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	float32 WinWindow::GetWindowTime() const
	{
		return static_cast<float32>(glfwGetTime());
	}

	void WinWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		FR_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			FR_ASSERT(success);
			glfwSetErrorCallback(GLFWErrorCallBack);
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// glfw ウインドウサイズ変更のコールバック
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallBack(event);
		});

		// glfw ウインドウ閉じるのコールバック
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallBack(event);
		});

		// glfw キー入力のコールバック
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 0);
				data.EventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(static_cast<KeyCode>(key));
				data.EventCallBack(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 1);
				data.EventCallBack(event);
				break;
			}
			}
		});

		// glfw キータイプのコールバック
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			data.EventCallBack(event);
		});

		// glfw マウス入力のコールバック
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				data.EventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
				data.EventCallBack(event);
				break;
			}
			}
		});

		// glfw マウススクロールのコールバック
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallBack(event);
		});

		// glfw マウス座標コールバック
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallBack(event);
		});
	}

	void WinWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);

		if (--s_GLFWWindowCount == 0)
		{
			FR_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}
}