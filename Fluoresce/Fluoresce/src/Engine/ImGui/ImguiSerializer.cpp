//==============================================================================//
// Name : ImguiSerializer.cpp													// 
// Describe : 	Imguiシリアライザ												// 
// Author : Ding Qi																// 
// Create Date : 2022/05/29														// 
// Modify Date : 2022/05/29														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/ImGui/ImguiSerializer.h"

#include "Engine/Core/Application.h"

#include <yaml-cpp/yaml.h>
#include <imgui.h>
#include <fstream>

namespace YAML {

	template<>
	struct convert<Fluoresce::FrVec4>
	{
		static Node encode(const Fluoresce::FrVec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, Fluoresce::FrVec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
};

namespace Fluoresce {

	static const std::array<const char*, 18> s_ColorName[]
	{
		"Color_WindowBg",
		"Color_Header",
		"Color_HeaderHovered",
		"Color_HeaderActive",
		"Color_Button",
		"Color_ButtonHovered",
		"Color_ButtonActive",
		"Color_FrameBg",
		"Color_FrameBgHovered",
		"Color_FrameBgActive",
		"Color_Tab",
		"Color_TabHovered",
		"Color_TabActive",
		"Color_TabUnfocused",
		"Color_TabUnfocusedActive",
		"Color_TitleBg",
		"Color_TitleBgActive",
		"Color_TitleBgCollapsed",
	};

	YAML::Emitter& operator<<(YAML::Emitter& out, const FrVec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void ImguiSerializer::Serialize(const std::string& filepath, sint32 fontIndex, const ImguiLayer::CostomItemColor& colors)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Editor SystemFont" << YAML::Value << fontIndex;
		out << YAML::Key << "Editor CostomColor" << YAML::Value << YAML::BeginSeq;
		uint32 index = 0;
		for (auto& color : colors)
		{
			out << YAML::BeginMap;
			out << YAML::Key << s_ColorName->at(index) << YAML::Value << colors.at(index);
			out << YAML::EndMap;
			index++;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		FR_CORE_TRACE("Serialized .config file path = {0}", filepath);

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool ImguiSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			FR_CORE_ERROR("Failed to load .config file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		auto pImguiLayer = Application::Get().GetImguiLayer();

		if (pImguiLayer)
		{
			auto font = data["Editor SystemFont"];
			if (font)
			{
				uint32 index = data["Editor SystemFont"].as<uint32>();
				pImguiLayer->SetFont(index);
			}

			auto colors = data["Editor CostomColor"];
			if (colors)
			{
				ImguiLayer::CostomItemColor itemColor;

				uint32 index = 0;
				for (auto& color : colors)
				{
					auto& key = s_ColorName->at(index);
					itemColor.at(index) = color[key].as<FrVec4>();
					index++;
				}
				pImguiLayer->SetItemColor(itemColor);
			}

			FR_CORE_TRACE("Deserializing .config file path = '{0}'", filepath);
			return true;
		}

		return false;
	}
}