project "zlib"
  kind "StaticLib"
  language "C++"
  staticruntime "On"

  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
      "*.c",
      "*.h"
  }
  
  filter "system:linux"
    pic "On"
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
