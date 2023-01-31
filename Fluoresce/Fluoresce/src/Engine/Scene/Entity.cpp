//==============================================================================//
// Name : Entity.cpp															// 
// Describe : 	エンティティ													// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2022/12/29														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/Entity.h"

namespace Fluoresce {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}

}