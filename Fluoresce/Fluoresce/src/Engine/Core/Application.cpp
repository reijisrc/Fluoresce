//==============================================================================//
// Name : Application.cpp														// 
// Describe : 	アプリケーション												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"
#include <glad/glad.h>

namespace Fluoresce {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		FR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallBack(FR_BIND_EVENT_FN(Application::OnEvent));

#ifdef FR_DEBUG
		FR_CORE_INFO("Application Initialized:DEBUG");
#elif FR_DEVELOPMENT
		FR_CORE_INFO("Application Initialized:DEVELOPMENT");
#elif FR_RELEASE
		FR_CORE_INFO("Application Initialized:RELEASE");
#endif
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FR_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(FR_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.8, 0.25, 0.4, 1.0);
			float32 time = m_Window->GetWindowTime();
			DeltaTime deltatime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(deltatime);
				}
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		return false;
	}

};