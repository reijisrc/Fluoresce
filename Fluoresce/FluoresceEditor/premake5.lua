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
		"%{wks.location}/Fluoresce/src"
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

   filter "configurations:Development"
		defines "FR_DEVELOPMENT"
		runtime "Release"
		optimize "on"

   filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"