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
		"src/**.cpp",
		"dependencies/stb_image/src/**.h",
		"dependencies/stb_image/src/**.cpp",
		"dependencies/ImGuizmo/src/ImGuizmo.h",
		"dependencies/ImGuizmo/src/ImGuizmo.cpp"
	}

	-- プリプロセッサ
  	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}
	
	-- 追加のインクルードディレクトリ
	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}
   
   -- 追加のライブラリディレクトリ
   libdirs       
   { 
		"%{LibraryDir.spdlog}",
		"%{LibraryDir.glfw}",
		"%{LibraryDir.glad}",
		"%{LibraryDir.glm}",
		"%{LibraryDir.imgui}",
		"%{LibraryDir.yaml_cpp}"
   }
   
   -- 外部プロジェクトとリンク
   links
   {
		"opengl32.lib"
   }
   
   filter "files:dependencies/ImGuizmo/src/**.cpp"
	flags { "NoPCH" }

   filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		
   filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"
		
		links
		{
			"spdlog_debug.lib",
			"glfw3_debug.lib",
			"glad_debug.lib",
			"glm_static_debug.lib",
			"imgui_debug.lib",
			"yaml-cpp_debug.lib"
		}
		
   filter "configurations:Development"
		defines "FR_DEVELOPMENT"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlog.lib",
			"glfw3_dev.lib",
			"glad.lib",
			"glm_static_dev.lib",
			"imgui.lib",
			"yaml-cpp.lib"
		}

   filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"
		
		links
		{
			"spdlog.lib",
			"glfw3.lib",
			"glad.lib",
			"glm_static.lib",
			"imgui.lib",
			"yaml-cpp.lib"
		}
		