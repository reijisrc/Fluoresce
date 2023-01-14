#pragma once
#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/Application.h"

#ifdef FR_PLATFORM_WINDOWS

extern Fluoresce::Application* Fluoresce::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Fluoresce::Log::Init();

	auto app = Fluoresce::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

#endif