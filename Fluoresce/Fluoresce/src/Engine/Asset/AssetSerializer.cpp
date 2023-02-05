//==============================================================================//
// Name : AssetSerializer.cpp													// 
// Describe : 	アセットシリアライザ											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Asset/AssetsManager.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "AssetSerializer.h"

namespace Fluoresce {

	static void SerializeTextureInfo(YAML::Emitter& out, std::pair<uint64, AssetsManager::TextureAsset> textureinfo)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << textureinfo.second.first.Name;
		out << YAML::Key << "Hash" << YAML::Value << textureinfo.first;
		out << YAML::Key << "Path" << YAML::Value << textureinfo.second.first.Path;
		out << YAML::EndMap;
	}

	void AssetSerializer::Serialize(const std::string& filepath, const std::string& scenename)
	{
		std::string str;
		Serialize(filepath, scenename, str);

		std::ofstream fout(filepath);
		fout << str;
	}

	void AssetSerializer::Serialize(const std::string& filepath, const std::string& scenename, std::string& outstr)
	{
		YAML::Emitter emitter;

		auto& textures = AssetsManager::Get().GetTextureAssets();
		if (!textures.empty())
		{
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Assets" << YAML::Value << YAML::BeginMap;
			emitter << YAML::Key << "Textures" << YAML::BeginSeq;
			for (auto texture : textures)
			{
				SerializeTextureInfo(emitter, texture);
			}
			emitter << YAML::EndSeq;
			emitter << YAML::EndMap;
			emitter << YAML::Newline;
			emitter << YAML::EndMap;

			outstr = emitter.c_str();
		}
	}

	bool AssetSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			FR_CORE_ERROR("Failed to load .scene file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		auto assets = data["Assets"];
		if (assets)
		{
			auto textures = assets["Textures"];
			if (textures)
			{
				for (auto texture : textures)
				{
					std::string name = texture["Name"].as<std::string>();
					std::string path = texture["Path"].as<std::string>();
					uint64 hash = texture["Hash"].as<uint64>();
					AssetsManager::Get().LoadTexture(path, name);
				}
			}
		}

		return true;
	}
}
