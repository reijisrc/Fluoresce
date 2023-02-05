//==============================================================================//
// Name : SceneScriptTask.h														// 
// Describe : 	シーンハンドル													// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#pragma once

#include "Engine/Scene/Entity.h"

namespace Fluoresce {

	// シーンスクリプトタスク
	class SceneScriptTask
	{
	public:
		using ScriptBindFn = std::function<void(Entity)>;
	public:
		SceneScriptTask() = default;

		void BuildNativeScript(const Ref<Scene>& scene, const ScriptBindFn& func);
		void OnSceneStart();
		void OnSceneUpdate(DeltaTime ts);
		void OnSceneEnd();

		static SceneScriptTask& Get();
	private:
		Ref<Scene> m_Context;
		friend class ScriptableEntity;
	};

}