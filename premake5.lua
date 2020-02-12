-- premake5.lua
workspace "QubGL"
	configurations { "Debug", "Release" }

project "QubGL"
	location "QubGL"
	kind "ConsoleApp"
	language "C++"
	architecture "x86_64"
	systemversion "latest"

	targetdir "bin/%{cfg.buildcfg}-%{cfg.architecture}"

	files { "QubGL/src/**.h", "QubGL/src/**.cpp" }

	includedirs {"Dependencies/GLEW/include", "Dependencies/GLFW/include", "Dependencies/GLM"}
	libdirs { "Dependencies/GLEW/lib"}

	if _ACTION == "vs2019" then
		libdirs { "Dependencies/GLFW/lib-vc2019"}
	elseif _ACTION == "vs2017" then
		libdirs { "Dependencies/GLFW/lib-vc2017"}
	elseif _ACTION == "vs2015" then
		libdirs { "Dependencies/GLFW/lib-vc2015"}
	end


	-- copy a file from the objects directory to the target directory
	postbuildcommands 	{
	"{RMDIR} %{cfg.targetdir}/resources",
	"{COPY} resources %{cfg.targetdir}/resources"}

	filter "system:windows"
		links { "glfw3", "glew32s", "opengl32"}
		defines {"WINDOWS", "GLEW_STATIC"}

	filter "system:linux"
		links { "GL", "glfw", "GLEW", "glut" }
		defines {"LINUX"}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
