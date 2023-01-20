//==============================================================================//
// Name : Application.h															// 
// Describe : 	�A�v���P�[�V����												// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Window.h"

#include "Engine//Events/Event.h"
#include "Engine//Events/ApplicationEvent.h"

int main(int argc, char** argv);

namespace Fluoresce {

	// �R���\�[������
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

	// �A�v���P�[�V���� �X�y�b�N
	struct ApplicationSpecification
	{
		std::string Name = "Fluoresce Engine";
		std::string WorkingDirectory;
		CommandLineArgs CmdLineArgs;
	};

	// �A�v���P�[�V����
	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		Window& GetWindow() { return *m_Window; }

		void Close();

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		ApplicationSpecification m_Specification;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// �A�v���P�[�V��������: ���sexe���Ɏ���
	Application* CreateApplication(CommandLineArgs args);
}
