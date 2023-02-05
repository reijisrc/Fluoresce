//==============================================================================//
// Name : AssetsManager.h														// 
// Describe :	アセットマネージャー											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Engine/Graphics/Texture.h"

namespace Fluoresce {

	struct TextureAssetInfo
	{
		std::string Name = ("Empty");
		std::string Path = ("");

		TextureAssetInfo()
			: Name("Empty"), Path("")
		{

		}

		TextureAssetInfo(const std::string& name, const std::string& path)
			: Name(name), Path(path)
		{

		}
	};

	class AssetsManager
	{
	public:
		using TextureAsset = std::pair<TextureAssetInfo, Ref<Texture2D>>;
		using TextureAssets = std::unordered_map<uint64, TextureAsset>;

		void ReleaseAssets();
		void ReleaseTextures();

		bool LoadTexture(const std::string& path, const std::string& name);

		bool GetTextureInfo(const std::string& name, TextureAssetInfo& out) const;
		bool GetTextureInfo(uint64 hash, TextureAssetInfo& out) const;

		WeakRef<Texture2D> GetTextureInstance(const std::string& name);
		WeakRef<Texture2D> GetTextureInstance(uint64 hash);

		const TextureAssets& GetTextureAssets() const { return m_TextureAssets; };

		static AssetsManager& Get();
	private:
		TextureAssets m_TextureAssets;
	};

}