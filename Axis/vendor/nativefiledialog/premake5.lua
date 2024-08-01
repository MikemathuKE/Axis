project "nfd"
	kind "StaticLib"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/*.h",
    "src/include/*.h",
    "src/nfd_common.c"
	}
  
  includedirs
  {
    "%{prj.location}/src/include"
  }
  
	filter "system:linux"
		pic "On"
    language "C"

		systemversion "latest"
		staticruntime "On"

		files
		{
			"src/nfd_zenity.c"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
    language "C++"

		files
		{
			"src/nfd_win.cpp"
		}
