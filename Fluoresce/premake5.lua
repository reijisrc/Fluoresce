include "Dependencies.lua"

workspace "Fluoresce"
	architecture "x86_64"
	startproject "FluoresceEditor"

	configurations
	{
		"Debug",
		"Development",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Fluoresce"
include "FluoresceEditor"