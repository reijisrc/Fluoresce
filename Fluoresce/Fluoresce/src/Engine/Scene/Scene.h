//==============================================================================//
// Name : Scene.h																// 
// Describe : 	シーン															// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Engine/Core/UniqueID.h"
#include "Engine/Core/DeltaTime.h"
#include "Engine/Graphics/EditorCamera.h"

#include <entt.hpp>

namespace Fluoresce {

	class Entity;

	namespace Editor {
		class SceneHierarchyPanel;
	}

	// シーン
	class Scene
	{
	public:
		using ScriptBindFn = std::function<void(Entity)>;
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		void BuildNativeScript(const ScriptBindFn& func);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUID(UniqueID uid, const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnEditorUpdate(DeltaTime ts);
		void OnRuntimeUpdate(DeltaTime ts);

		void OnEditorRender(DeltaTime ts, EditorCamera& camera);
		void OnRuntimeRender(DeltaTime ts);

		void OnViewportResize(uint32 width, uint32 height);

		void DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUID(UniqueID uid);
		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		std::unordered_map<UniqueID, entt::entity> m_EntityMap;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;

		friend class Entity;
		friend class SceneSerializer;
		friend class Editor::SceneHierarchyPanel;
	};
};