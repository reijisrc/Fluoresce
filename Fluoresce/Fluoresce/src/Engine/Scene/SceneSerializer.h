//==============================================================================//
// Name : SceneSerializer.h														// 
// Describe : 	シーンシリアライザ												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/08														// 
// Modify Date : 2023/01/08														// 
//==============================================================================//
#pragma once

#include "Engine/Scene/Scene.h"

namespace Fluoresce {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath, const std::string& scenename);
		bool Deserialize(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}