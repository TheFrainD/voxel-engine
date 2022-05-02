workspace "VoxelEngine"
    architecture "x86"
    startproject "VoxelEngine"

    configurations 
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "VoxelEngine/vendor/GLFW"
include "VoxelEngine/vendor/glad"

include "VoxelEngine"