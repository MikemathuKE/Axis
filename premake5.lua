workspace "Axis"
  architecture "x64"
  startproject "Axis-Editor"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

  flags
  {
    "MultiProcessorCompile"
  }
  
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories
IncludeDir = {}
IncludeDir["vendor"] = "%{wks.location}/Axis/vendor"
IncludeDir["spdlog"] = "%{wks.location}/Axis/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Axis/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Axis/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Axis/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Axis/vendor/glm"
IncludeDir["assimp"] = "%{wks.location}/Axis/vendor/assimp/include"
IncludeDir["entt"] = "%{wks.location}/Axis/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Axis/vendor/yaml/include"
IncludeDir["nfd"] = "%{wks.location}/Axis/vendor/nativefiledialog/src/include"
IncludeDir["im3d"] = "%{wks.location}/Axis/vendor/ImGuizmo"

group "Dependencies"
  include "Axis/vendor/GLFW"
  include "Axis/vendor/Glad"
  include "Axis/vendor/imgui"
  include "Axis/vendor/assimp"
  include "Axis/vendor/yaml"
  include "Axis/vendor/nativefiledialog"
group ""

project "Axis"
  location "Axis"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"
 
  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")
 
  pchheader "axispch.h"
  pchsource "Axis/src/axispch.cpp"
 
  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/stb_image/stb_image.cpp",
    "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
    "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
  }
 
  includedirs
  {
    "%{prj.name}/src",
    "%{IncludeDir.vendor}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.assimp}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.yaml_cpp}",
    "%{IncludeDir.nfd}",
    "%{IncludeDir.ImGuizmo}"
  }
  
  links
  {
    "GLFW",
    "Glad",
    "imgui",
    "assimp",
    "yaml-cpp"
  }
  
  defines
  {
    "GLFW_INCLUDE_NONE",
    "_CRT_SECURE_NO_WARNINGS"
  }
  
  filter "files:vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }
 
  filter "system:linux"
    pic "on"
    systemversion "latest"
  
    links
    {
      "Xrandr",
      "Xi",
      "GL",
      "X11",
      "OpenGL",
      "nfd"
    }
 
  filter "system:windows"
    systemversion "latest"
    
    links
    {
      "opengl32.lib"
    }
    
  filter "configurations:Debug"
    defines "AXIS_DEBUG"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "on"

  filter "configurations:Dist"
    defines "AXIS_DIST"
    runtime "Release"
    optimize "Full"

project "SandBox"
  location "SandBox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
   "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{wks.location}/Axis/src",
    "%{IncludeDir.vendor}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.assimp}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.ImGuizmo}"
  }
  
  links
  {
    "Axis",
    "ImGui"
  }
    

  filter "system:linux"
    systemversion "latest"
    
    links
    {
      "dl",
      "pthread",
      "Glad",
      "GLFW",
      "zlib",
      "assimp"
    }
    
  filter "system:windows"
    systemversion "latest"

  filter "configurations:Debug"
    defines "AXIS_DEBUG"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "on"

  filter "configurations:Dist"
    defines "AXIS_DIST"
    runtime "Release"
    optimize "Full"
    
  
project "Axis-Editor"
  location "Axis-Editor"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
   "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{wks.location}/Axis/src",
    "%{prj.location}/src",
    "%{IncludeDir.vendor}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.assimp}",
    "%{IncludeDir.entt}"
  }
  
  links
  {
    "Axis"
  }
    

  filter "system:linux"
    systemversion "latest"
    
    links
    {
      "dl",
      "pthread",
      "Glad",
      "GLFW",
      "imgui",
      "yaml-cpp",
      "nfd"
    }
    
  filter "system:windows"
    systemversion "latest"

  filter "configurations:Debug"
    defines "AXIS_DEBUG"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "on"

  filter "configurations:Dist"
    defines "AXIS_DIST"
    runtime "Release"
    optimize "Full"
