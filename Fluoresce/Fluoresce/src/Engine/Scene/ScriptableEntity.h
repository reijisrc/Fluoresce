//==============================================================================//
// Name : ScriptableEntity.h													// 
// Describe : 	�X�N���v�g�\�G���e�B�e�B										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Engine/Scene/Entity.h"

namespace Fluoresce {

	// �X�N���v�g�\�G���e�B�e�B
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
};
