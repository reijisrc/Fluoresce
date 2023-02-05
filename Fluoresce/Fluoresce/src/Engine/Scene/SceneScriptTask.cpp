//==============================================================================//
// Name : SceneScriptTask.cpp													// 
// Describe : 	シーンハンドル													// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/SceneScriptTask.h"
#include "Engine/Scene/ScriptableEntity.h"
#include "Engine/Scene/Entity.h"

namespace Fluoresce {

	Scope<SceneScriptTask> s_SceneScriptTask = CreateScope<SceneScriptTask>();

	void SceneScriptTask::BuildNativeScript(const Ref<Scene>& scene, const ScriptBindFn& func)
	{
		m_Context = scene;
		if (m_Context)
		{
			auto view = m_Context->m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, scene.get() };
				func(entity);
			}
		}
	}

	void SceneScriptTask::OnSceneStart()
	{
		// スクリプトコンストラクタ
		if(m_Context)
		{
			m_Context->m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nativeScript)
			{
				if (nativeScript.ScriptID != 0 && nativeScript.Instance == nullptr)
				{
					nativeScript.Instance = nativeScript.InstantiateScript();
					nativeScript.Instance->m_Entity = Entity{ entity, m_Context.get() };
					nativeScript.Instance->Init();
				}
			});

			m_SortUpdateTask = true;
		}
	}

	void SceneScriptTask::OnSceneUpdate(DeltaTime ts)
	{
		if (m_SortUpdateTask)
		{
			m_UpdateTask.clear();
		}

		// スクリプト更新
		if (m_Context)
		{
			m_Context->m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nativeScript)
			{
				if (nativeScript.Instance != nullptr)
				{
					if(nativeScript.Instance->m_State == ScriptableEntityState::Initializated)
					{
						nativeScript.Instance->OnCreate();
						nativeScript.Instance->m_State = ScriptableEntityState::Active;
					}
					
					//if(nativeScript.Instance->m_State == ScriptableEntityState::Active)
					//{
					//	nativeScript.Instance->OnUpdate(ts);
					//}
					
					if(nativeScript.Instance->m_State == ScriptableEntityState::Destroy)
					{
						nativeScript.Instance->OnDestroy();
						nativeScript.DestroyScript(&nativeScript);
					}

					if (m_SortUpdateTask)
					{
						if (nativeScript.Instance->m_State == ScriptableEntityState::Active)
						{
							m_UpdateTask.push_back(nativeScript.Instance);
						}
					}
				}
			});

			// 更新タスクソート
			if (m_SortUpdateTask)
			{
				std::sort(m_UpdateTask.begin(), m_UpdateTask.end(), [](ScriptableEntity* l, ScriptableEntity* r)
				{
					return l->GetUpdatePriority() < r->GetUpdatePriority();
				});

				m_SortUpdateTask = false;
			}

			// 更新
			for (auto& script : m_UpdateTask)
			{
				script->OnUpdate(ts);
			}
		}
	}

	void SceneScriptTask::OnSceneEnd()
	{
		// スクリプトデストラクタ
		if (m_Context)
		{
			m_Context->m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nativeScript)
			{
				if (nativeScript.Instance != nullptr)
				{
					nativeScript.Instance->OnDestroy();
					nativeScript.DestroyScript(&nativeScript);
				}
			});
		}
	}

	SceneScriptTask& SceneScriptTask::Get()
	{
		return *s_SceneScriptTask;
	}
}