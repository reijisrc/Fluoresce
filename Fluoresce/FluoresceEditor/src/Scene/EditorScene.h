//==============================================================================//
// Name : EditorScene.h															// 
// Describe : 	エディターシーン												// 
// Author : Ding Qi																// 
// Create Date : 2023/02/10														// 
// Modify Date : 2023/02/10														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		// シーン
		class EditorScene : public Scene
		{
		public:
			EditorScene();
			~EditorScene();

			static Ref<EditorScene> Copy(Ref<EditorScene> other);

			void OnRuntimeStart();
			void OnRuntimeStop();

			void OnEditorUpdate(DeltaTime ts);
			void OnRuntimeUpdate(DeltaTime ts);

			void OnEditorRender(DeltaTime ts, EditorCamera& camera);
			void OnRuntimeRender(DeltaTime ts);

			bool IsRunning() const { return m_IsRunning; }
			bool IsPaused() const { return m_IsPaused; }

			void SetPaused(bool paused) { m_IsPaused = paused; }
		protected:
			entt::registry& GetRegistry() { return m_Registry; }
		private:
			bool m_IsRunning = false;
			bool m_IsPaused = false;

			friend class SceneHierarchyPanel;
		};
	}

}