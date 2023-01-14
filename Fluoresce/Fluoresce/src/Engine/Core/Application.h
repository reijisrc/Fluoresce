#pragma once

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Assert.h"

int main(int argc, char** argv);

namespace Fluoresce {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			FR_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Fluoresce Engine";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Close();

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

	private:
		void Run();
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = true;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// アプリケーション側実装
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
