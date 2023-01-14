project "FluoresceEditor"
  kind "ConsoleApp"
  language "C++"
  toolset "v142"
  cppdialect "C++17"
  staticruntime "on"
  
  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	
   includedirs
   {
		"%{wks.location}/Fluoresce/dependencies/spdlog/include",
		"%{wks.location}/Fluoresce/src"
   }
   
    libdirs 
   { 
		"%{LibraryDir.spdlog}"
   }
   
   links
   {
		"Fluoresce"
   }

   filter "system:windows"
		systemversion "latest"

   filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"
		
		links
		{
			"spdlogd.lib"
		}

   filter "configurations:Development"
		defines "FR_DEVELOPMENT"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlog.lib"
		}

   filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlog.lib"
		}
		