//==============================================================================//
// Name : ScriptableEntity.cpp													// 
// Describe : 	スクリプト可能エンティティ										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/05														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/ScriptableEntity.h"

namespace Fluoresce {

	void ScriptableEntity::BindScript(Entity entity, uint32 scriptId, const SceneScriptTask::ScriptBindFn& func)
	{
		auto& scriptTask = SceneScriptTask::Get();
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& nativescriptComponent = entity.GetComponent<ScriptComponent>();
			nativescriptComponent.ScriptID = scriptId;

			func(entity);
			
			if (nativescriptComponent.ScriptID != 0 && nativescriptComponent.Instance == nullptr)
			{
				nativescriptComponent.Instance = nativescriptComponent.InstantiateScript();
				nativescriptComponent.Instance->m_Entity = Entity{ entity, scriptTask.m_Context.get() };
				nativescriptComponent.Instance->Init();
			}

			scriptTask.m_RemapUpdateTask = true;
		}
	}

	void ScriptableEntity::DestoryScriptableEntity(Entity entity)
	{
		auto& scriptTask = SceneScriptTask::Get();
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& nativescriptComponent = entity.GetComponent<ScriptComponent>();
			if (nativescriptComponent.Instance != nullptr)
			{
				nativescriptComponent.Instance->m_State = ScriptableEntityState::Destroy;
			}

			scriptTask.m_RemapUpdateTask = true;
		}
	}

	Entity ScriptableEntity::CreateEntity(const std::string& name)
	{
		auto& scriptTask = SceneScriptTask::Get();
		return scriptTask.m_Context->CreateEntity(name);
	}

	void ScriptableEntity::DestroyEntity(Entity entity)
	{
		auto& scriptTask = SceneScriptTask::Get();
		scriptTask.m_Context->DestroyEntity(entity);
		scriptTask.m_RemapUpdateTask = true;
	}

	Entity ScriptableEntity::FindEntityByName(std::string_view name)
	{
		auto& scriptTask = SceneScriptTask::Get();
		return scriptTask.m_Context->FindEntityByName(name);
	}

	Entity ScriptableEntity::GetEntityByUID(UniqueID uid)
	{
		auto& scriptTask = SceneScriptTask::Get();
		return scriptTask.m_Context->GetEntityByUID(uid);
	}

	Entity ScriptableEntity::GetPrimaryCameraEntity()
	{
		auto& scriptTask = SceneScriptTask::Get();
		return scriptTask.m_Context->GetPrimaryCameraEntity();
	}

	void ScriptableEntity::SetUpdatePriority(uint32 prio)
	{
		auto& scriptTask = SceneScriptTask::Get();

		m_UpdatePriority = prio;
		scriptTask.m_RemapUpdateTask = true;
	}

	void ScriptableEntity::Init()
	{
		if (m_Entity)
		{
			m_UpdatePriority = GetDefaultUpdatePriority();
			m_State = ScriptableEntityState::Initializated;
		}
	}

	void ScriptableEntity::SetRemove()
	{
		if (m_Entity)
		{
			m_State = ScriptableEntityState::Destroy;
		}
	}
}