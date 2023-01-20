//==============================================================================//
// Name : Editor.cpp															// 
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