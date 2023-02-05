//==============================================================================//
// Name : SnowParticleManager.h													// 
// Describe : 	スクリプト:雪パーティクルマネージャー							// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "NativeScript/SnowParticleManager.h"
#include "NativeScript/ScriptTaskHandle.h"

namespace Fluoresce {

	void SnowParticleManager::OnCreate()
	{
		FR_CLIENT_TRACE("ScriptSnowParticle Create");
	}

	void SnowParticleManager::OnDestroy()
	{
		FR_CLIENT_TRACE("ScriptSnowParticle Destroy");
	}

	void SnowParticleManager::OnUpdate(DeltaTime ts)
	{
		if (m_Timer >= 1.0f)
		{
			// 雪パーティクル生成
			CreateSnowEntity();
			m_Timer = 0.0f;
		}
		else
		{
			m_Timer += ts;
		}
	}

	void SnowParticleManager::CreateSnowEntity()
	{
		static uint32 count = 0;
		static const std::string entityname = "Snow";
		static const std::string snow01name = "snow_01";
		static const std::string snow02name = "snow_02";
		static const std::string snow03name = "snow_03";

		std::string strname = entityname;
		std::string strcount = std::to_string(count);
		strname += strcount;

		auto entity = CreateEntity(strname);
		entity.AddComponent<SpriteRendererComponent>();
		entity.AddComponent<ScriptComponent>();
		uint32 snowTex = (rand() % 100);
		float32 startPosX = (rand() % 80) * 0.125f - 5.0f;
		float32 startRotZ = (rand() % 100) / 50.f * Float::PI;
		float32 startScale = 0.25f + (rand() % 100) * 0.0075f;

		auto& transformComponent = entity.GetComponent<TransformComponent>();
		transformComponent.Translation = Vec3(startPosX, 4.0f, 0.0f);
		transformComponent.Rotation = Vec3(0.0f, 0.0f, startRotZ);
		transformComponent.Scale = Vec3(startScale, startScale, 0.0f);

		auto& spriteComponent = entity.GetComponent<SpriteRendererComponent>();
		if (snowTex < 30)
		{
			spriteComponent.SetTextureAsset(snow01name);
		}
		else if (snowTex < 70)
		{
			spriteComponent.SetTextureAsset(snow02name);
		}
		else
		{
			spriteComponent.SetTextureAsset(snow03name);
		}

		auto& scriptComponent = entity.GetComponent<ScriptComponent>();
		BindScript(entity, Editor::ScriptID::_SnowParticle, Editor::ScriptTaskHandle::BindScript);
		count++;
	}
}
