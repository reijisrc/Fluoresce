//==============================================================================//
// Name : Application.cpp														// 
// Describe : 	アプリケーション												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/22														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Core/Application.h"

namespace Fluoresce {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		FR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		FR_CORE_INFO("Application Initialized");
	}

	Application::~Application()
	{

	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// FR_CORE_TRACE("Update");
		}
	}

};