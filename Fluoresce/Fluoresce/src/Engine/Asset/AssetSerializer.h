//==============================================================================//
// Name : AssetSerializer.h														// 
// Describe : 	アセットシリアライザ											// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	class AssetSerializer
	{
	public:
		void Serialize(const std::string& filepath, const std::string& scenename);
		void Serialize(const std::string& filepath, const std::string& scenename, std::string& outstr);

		bool Deserialize(const std::string& filepath);
	};
}