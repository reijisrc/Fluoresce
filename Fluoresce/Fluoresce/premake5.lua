project "Fluoresce"
	kind "StaticLib"
	language "C++"
	toolset "v142"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "frpch.h"
	pchsource "src/frpch.cpp"
	
	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

  	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}
	
	includedirs
   {
		"src",
		"%{IncludeDir.spdlog}"
   }

   links
   {
		"opengl32.lib"
   }

   filter "system:windows"
		systemversion "latest"

   filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"
		
   filter "configurations:Development"
		defines "FR_DEVELOPMENT"
		runtime "Release"
		optimize "on"

   filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"