project "FluoresceEditor"
  kind "ConsoleApp"
  language "C++"
  toolset "v142"
  cppdialect "C++17"
  
  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	

	-- 追加のインクルードディレクトリ
   includedirs
   {
		"src",
		"%{wks.location}/Fluoresce/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}"
   }
   
   -- 外部プロジェクトとリンク
   links
   {
		"Fluoresce"
   }

   filter "system:windows"
		systemversion "latest"
		staticruntime "on"
		
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
		