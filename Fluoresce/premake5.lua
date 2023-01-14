workspace "Fluoresce"
	architecture "x86_64"
	startproject "FluoresceEditorn"

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