//==============================================================================//
// Name : FluoresceApp.cpp														// 
// Describe : 	アプリケーション(エディター)									// 
// Author : Ding Qi																// 
// Create Date : 2022/05/14														// 
// Modify Date : 2022/05/14														// 
//==============================================================================//
#include "Fluoresce.h"
#include "Engine/Core/EnterPoint.h"

#include "EditorLayer.h"

namespace Fluoresce {

	class FluoresceApp : public Application
	{
	public:
		FluoresceApp(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication(CommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "FluoresceEditor";
		spec.CmdLineArgs = args;

		return new FluoresceApp(spec);
	}

}