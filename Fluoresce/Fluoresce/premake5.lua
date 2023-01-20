project "Fluoresce"
	kind "StaticLib"
	language "C++"
	toolset "v142"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "frpch.h"
	pchsource "src/frpch.cpp"
	
	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

	-- プリプロセッサ
  	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	-- 追加のインクルードディレクトリ
	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}"
	}
   
   -- 追加のライブラリディレクトリ
   libdirs       
   { 
		"%{LibraryDir.spdlog}",
		"%{LibraryDir.glfw}",
		"%{LibraryDir.glad}"
   }
   
   -- 外部プロジェクトとリンク
   links
   {
		"opengl32.lib"
   }

   filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		
   filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"
		
		links
		{
			"spdlogd.lib",
			"glfw3_debug.lib",
			"glad_debug.lib"
		}
		
   filter "configurations:Development"
		defines "FR_DEVELOPMENT"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlogd.lib",
			"glfw3_dev.lib",
			"glad_debug.lib"
		}

   filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlog.lib",
			"glfw3.lib",
			"glad.lib"
		}
		