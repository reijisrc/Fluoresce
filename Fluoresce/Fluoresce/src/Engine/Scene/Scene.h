//==============================================================================//
// Name : Scene.h																// 
// Describe : 	シーン															// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2023/02/10														// 
//==============================================================================//
#pragma once

#include "Engine/Core/UniqueID.h"
#include "Engine/Core/DeltaTime.h"
#include "Engine/Graphics/EditorCamera.h"

#include <entt.hpp>

namespace Fluoresce {

	class Entity;

	namespace Editor {
		class EditorScene;
	}

	// シーン
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();


		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUID(UniqueID uid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		void OnUpdate(DeltaTime ts);
		void OnRender(DeltaTime ts);

		void OnViewportResize(uint32 width, uint32 height);

		void DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUID(UniqueID uid);
		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		std::unordered_map<UniqueID, entt::entity> m_EntityMap;

		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneScriptTask;
		friend class Editor::EditorScene;
	};
};