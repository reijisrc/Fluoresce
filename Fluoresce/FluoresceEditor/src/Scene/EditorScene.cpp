//==============================================================================//
// Name : EditorScene.cpp														// 
// Describe : 	エディターシーン												// 
// Author : Ding Qi																// 
// Create Date : 2023/02/10														// 
// Modify Date : 2023/02/10														// 
//==============================================================================//
#include "Scene/EditorScene.h"

namespace Fluoresce {

	namespace Editor
	{
		template<typename Component>
		static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UniqueID, entt::entity>& enttMap)
		{
			auto view = src.view<Component>();
			for (auto e : view)
			{
				UniqueID uid = src.get<IDComponent>(e).ID;
				FR_CORE_ASSERT(enttMap.find(uid) != enttMap.end(), "Can not find uid!");
				entt::entity dstEnttID = enttMap.at(uid);

				auto& component = src.get<Component>(e);
				dst.emplace_or_replace<Component>(dstEnttID, component);
			}
		}

		EditorScene::EditorScene()
		{

		}

		EditorScene::~EditorScene()
		{

		}

		Ref<EditorScene> EditorScene::Copy(Ref<EditorScene> other)
		{
			Ref<EditorScene> newScene = CreateRef<EditorScene>();

			newScene->m_ViewportWidth = other->m_ViewportWidth;
			newScene->m_ViewportHeight = other->m_ViewportHeight;

			auto& srcSceneRegistry = other->m_Registry;
			auto& dstSceneRegistry = newScene->m_Registry;
			std::unordered_map<UniqueID, entt::entity> enttMap;

			// エンティティ再作成
			auto idView = srcSceneRegistry.view<IDComponent>();
			for (auto e : idView)
			{
				UniqueID uid = srcSceneRegistry.get<IDComponent>(e).ID;
				const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
				Entity newEntity = newScene->CreateEntityWithUID(uid, name);
				enttMap[uid] = (entt::entity)newEntity;
			}

			// コンポーネントコピー
			CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
			CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
			CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
			CopyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

			return newScene;
		}

		void EditorScene::OnRuntimeStart()
		{
			auto& sceneScriptTask = SceneScriptTask::Get();
			sceneScriptTask.OnSceneStart();
		}

		void EditorScene::OnRuntimeStop()
		{
			auto& sceneScriptTask = SceneScriptTask::Get();
			sceneScriptTask.OnSceneEnd();
		}

		void EditorScene::OnEditorUpdate(DeltaTime ts)
		{
			if (IsPaused())
			{
				return;
			}
		}

		void EditorScene::OnRuntimeUpdate(DeltaTime ts)
		{
			if (IsPaused())
			{
				return;
			}

			auto& sceneScriptTask = SceneScriptTask::Get();
			sceneScriptTask.OnSceneUpdate(ts);
		}

		void EditorScene::OnEditorRender(DeltaTime ts, EditorCamera& camera)
		{
			auto& spriteRenderer = RenderPipeline::GetSpriteRenderer();
			auto& skyboxRenderer = RenderPipeline::GetSkyboxRenderer();

			skyboxRenderer.Submit(camera);

			spriteRenderer.Begin(camera);

			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					spriteRenderer.DrawSpriteEntity(transform.GetTransform(), sprite);
				}
			}

			spriteRenderer.End();
		}

		void EditorScene::OnRuntimeRender(DeltaTime ts)
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
				auto& skyboxRenderer = RenderPipeline::GetSkyboxRenderer();

				skyboxRenderer.Submit(*mainCamera, cameraTransform);

				spriteRenderer.Begin(*mainCamera, cameraTransform);

				{
					auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
					for (auto entity : group)
					{
						auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

						spriteRenderer.DrawSpriteEntity(transform.GetTransform(), sprite);
					}
				}

				spriteRenderer.End();
			}

		}
	}

}