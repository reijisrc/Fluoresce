//==============================================================================//
// Name : Scene.cpp																// 
// Describe : 	シーン															// 
// Author : Ding Qi																// 
// Create Date : 2022/12/29														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#include "frpch.h"
#include "Engine/Scene/Scene.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/RenderPipeline.h"
#include "Engine/Scene/ScriptableEntity.h"

namespace Fluoresce {

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UniqueID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UniqueID uuid = src.get<IDComponent>(e).ID;
			FR_CORE_ASSERT(enttMap.find(uuid) != enttMap.end(), "Can not find uid!");
			entt::entity dstEnttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UniqueID, entt::entity> enttMap;

		// エンティティ再作成
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UniqueID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// コンポーネントコピー
		CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	void Scene::BuildNativeScript(const ScriptBindFn& func)
	{
		auto view = m_Registry.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			func(entity);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUID(UniqueID(), name);
	}

	Entity Scene::CreateEntityWithUID(UniqueID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		// スクリプトコンストラクタ
		{
			m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.ScriptID != 0 && nsc.Instance == nullptr)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
			});
		}
	}

	void Scene::OnRuntimeStop()
	{
		// スクリプトデストラクタ
		{
			m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.Instance != nullptr)
				{
					nsc.Instance->OnDestroy();
					nsc.DestroyScript(&nsc);
				}
			});
		}
	}

	void Scene::OnEditorUpdate(DeltaTime ts)
	{
		if (m_IsPaused)
		{
			return;
		}
	}

	void Scene::OnRuntimeUpdate(DeltaTime ts)
	{
		if (m_IsPaused)
		{
			return;
		}

		// スクリプト更新
		m_Registry.view<ScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (nsc.Instance != nullptr)
			{
				nsc.Instance->OnUpdate(ts);
			}
		});
	}

	void Scene::OnEditorRender(DeltaTime ts, EditorCamera& camera)
	{
		auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

		spriteRenderer.Begin(camera);

		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				spriteRenderer.DrawSpriteEntity(transform.GetTransform(), sprite, (sint32)entity);
			}
		}

		spriteRenderer.End();
	}

	void Scene::OnRuntimeRender(DeltaTime ts)
	{
		// メインカメラ取得
		Camera* mainCamera = nullptr;
		Mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();

			spriteRenderer.Begin(*mainCamera, cameraTransform);

			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					spriteRenderer.DrawSpriteEntity(transform.GetTransform(), sprite, (sint32)entity);
				}
			}

			spriteRenderer.End();
		}

	}

	void Scene::OnViewportResize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<ScriptComponent>(newEntity, entity);
	}

	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUID(UniqueID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{

	}
}