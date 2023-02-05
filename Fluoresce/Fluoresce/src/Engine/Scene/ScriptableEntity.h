//==============================================================================//
// Name : ScriptableEntity.h													// 
// Describe : 	スクリプト可能エンティティ										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/02/05														// 
//==============================================================================//
#pragma once

#include "Engine/Scene/SceneScriptTask.h"
#include "Engine/Scene/Entity.h"

namespace Fluoresce {

	enum class ScriptableEntityState
	{
		Initializated = 0,
		Active,
		Destroy,
		Undefine,
	};

	// スクリプト可能エンティティ
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		void BindScript(Entity entity, uint32 scriptId, const SceneScriptTask::ScriptBindFn& func);
		void DestoryScriptableEntity(Entity entity);

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUID(UniqueID uid);
		Entity GetPrimaryCameraEntity();

		void SetUpdatePriority(uint32 prio);
		uint32 GetUpdatePriority() const
		{
			return m_UpdatePriority;
		}

		virtual uint32 GetDefaultUpdatePriority() const { return 0; }
	protected:
		virtual void Init();

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime ts) {}
	private:
		Entity m_Entity;
		ScriptableEntityState m_State = ScriptableEntityState::Undefine;
		uint32 m_UpdatePriority = 0;
		friend class SceneScriptTask;
	};
};
