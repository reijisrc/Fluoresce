//==============================================================================//
// Name : Editor.cpp														// 
// Describe : 	アプリケーション:エディター										// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/03/22														// 
//==============================================================================//

#include <Fluoresce.h>
#include <Engine/Core/EnterPoint.h>

namespace Fluoresce {

	class FluoresceEditor : public Application
	{
	public:
		FluoresceEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			FR_CLIENT_INFO("Editor Initialized");
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "FluoresceEditor";
		spec.CommandLineArgs = args;

		return new FluoresceEditor(spec);
	}

}

//int main()
//{
//	Fluoresce::Log::Init();
//
//	FR_CLIENT_INFO("Console Application");
//
//	int t = 5;
//	FR_CORE_ASSERT(t > 3);
//	FR_CLIENT_TRACE("t = {0}", t);
//
//	std::cin.get();
//}
