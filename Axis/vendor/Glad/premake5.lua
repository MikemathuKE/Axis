project "Glad"
  kind "StaticLib"
  language "C"
  staticruntime "on"

  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "include/glad/glad.h",
    "include/KHR/khrplatform.h",
    "src/glad.c"
  }
  
  includedirs
  {
    "include"
  }
  
  filter "system:linux"
    pic "on"
    systemversion "latest"
    
  filter "system:windows"
    systemversion "latest"

  filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    runtime "Release"
    optimize "on"
  
  filter "configurations:Dist"
    runtime "Release"
    optimize "Full"
