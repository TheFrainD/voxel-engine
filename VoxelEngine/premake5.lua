project "VoxelEngine"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vepch.h"
	pchsource "src/vepch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    includedirs
	{
		"src",
        "src",
		"vendor/glfw/include",
		"vendor/glad/include",
		"vendor/spdlog/include",
		"vendor/glm",
		"vendor/stb",
		"vendor/FastNoiseLite/Cpp"
	}

    links
	{
		"GLFW",
		"glad"
	}

    filter "system:linux"
        links { "dl", "pthread" }
        defines { "__linux__" }

    filter "system:windows"
		links { "opengl32.lib" }
        defines { "_WIN32" }

	filter "configurations:Debug"
		defines "VE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		kind "WindowedApp"
		runtime "Release"
		optimize "on"