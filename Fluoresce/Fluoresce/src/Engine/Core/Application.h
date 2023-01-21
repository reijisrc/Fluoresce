//==============================================================================//
// Name : Application.h															// 
// Describe : 	アプリケーション												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/DataDefine.h"

#include "Engine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Fluoresce {

	// コンソール引数
	struct CommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			FR_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	// アプリケーション スペック
	struct ApplicationSpecification
	{
		std::string Name = "Fluoresce";
		std::string WorkingDirectory;
		CommandLineArgs CmdLineArgs;
	};

	// アプリケーション
	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack	m_LayerStack;
		float32		m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;

		friend int ::main(int argc, char** argv);
	};

	// アプリケーション入口: 実行exe側に実装
	Application* CreateApplication(CommandLineArgs args);
}
