//==============================================================================//
// Name : AssetsManager.cpp														// 
// Describe :	アセットマネージャー											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Asset/AssetsManager.h"

namespace Fluoresce {

	Scope<AssetsManager> s_AssetManager = CreateScope<AssetsManager>();

	void AssetsManager::ReleaseAssets()
	{
		ReleaseTextures();
	}

	void AssetsManager::ReleaseTextures()
	{
		for (auto& texture : m_TextureAssets) {
			auto name = texture.second.first.Name;
			FR_CORE_TRACE("Release texture asset: {0}", name);
		}
		m_TextureAssets.clear();
	}

	bool AssetsManager::LoadTexture(const std::string& path, const std::string& name)
	{
		uint64 hash = std::hash<std::string>()(name);
		if (m_TextureAssets.find(hash) != m_TextureAssets.end())
		{
			FR_CORE_WARN("Texture already exist!");
			return nullptr;
		}

		auto texture = Texture2D::Create(path);
		if (texture)
		{
			TextureAssetInfo info{ name, path };
			m_TextureAssets.emplace(std::make_pair(hash, std::make_pair(info, std::move(texture))));
			FR_CORE_TRACE("Load texture asset hash = {0} name = {1}\n Path = {2}", hash, name, path);
			return true;
		}
		return false;
	}

	bool AssetsManager::GetTextureInfo(const std::string& name, TextureAssetInfo& out) const
	{
		uint64 hash = std::hash<std::string>()(name);
		return GetTextureInfo(hash, out);
	}

	bool AssetsManager::GetTextureInfo(uint64 hash, TextureAssetInfo& out) const
	{
		auto it = m_TextureAssets.find(hash);
		if (it != m_TextureAssets.end())
		{
			out = it->second.first;
			return true;
		}
		return false;
	}

	WeakRef<Texture2D> AssetsManager::GetTextureInstance(const std::string& name)
	{
		uint64 hash = std::hash<std::string>()(name);
		return GetTextureInstance(hash);
	}

	WeakRef<Texture2D> AssetsManager::GetTextureInstance(uint64 hash)
	{
		auto it = m_TextureAssets.find(hash);
		if (it != m_TextureAssets.end())
		{
			return WeakRef<Texture2D>(it->second.second);
		}
		return WeakRef<Texture2D>();
	}

	AssetsManager& AssetsManager::Get()
	{
		return *s_AssetManager;
	}
}