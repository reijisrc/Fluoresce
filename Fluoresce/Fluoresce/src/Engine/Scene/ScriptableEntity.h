//==============================================================================//
// Name : ScriptableEntity.h													// 
// Describe : 	スクリプト可能エンティティ										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Engine/Scene/Entity.h"

namespace Fluoresce {

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
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
};
