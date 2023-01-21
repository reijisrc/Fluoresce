//==============================================================================//
// Name : Editor.cpp															// 
// Describe : 	Fluoresceエディター												// 
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
#ifdef FR_DEBUG
			FR_CLIENT_INFO("Application(DEBUG) Initialized");
#elif FR_DEVELOPMENT
			FR_CLIENT_INFO("Application(DEVELOPMENT) Initialized");
#elif FR_RELEASE
			FR_CLIENT_INFO("Application(RELEASE) Initialized");
#endif
		}
	};

	Application* CreateApplication(CommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "FluoresceEditor";
		spec.CmdLineArgs = args;

		return new FluoresceEditor(spec);
	}

}