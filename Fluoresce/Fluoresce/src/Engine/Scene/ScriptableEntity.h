//==============================================================================//
// Name : ScriptableEntity.h													// 
// Describe : 	�X�N���v�g�\�G���e�B�e�B										// 
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

	// �X�N���v�g�\�G���e�B�e�B
	class ScriptableEntity
	{
	public:
		ScriptableEntity();
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void BindScript(Entity entity, uint32 scriptId, const SceneScriptTask::ScriptBindFn& func);
		void DestoryScriptableEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUID(UniqueID uid);
		Entity GetPrimaryCameraEntity();

		void SetUpdatePriority(uint32 prio);
		uint32 GetUpdatePriority() const
		{
			return m_UpdatePriority;
		}
	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(DeltaTime ts) {}

		virtual uint32 GetDefaultUpdatePriority() { return 0; }
	private:
		Entity m_Entity;
		ScriptableEntityState m_State = ScriptableEntityState::Undefine;
		uint32 m_UpdatePriority = 0;
		friend class SceneScriptTask;
	};
};
