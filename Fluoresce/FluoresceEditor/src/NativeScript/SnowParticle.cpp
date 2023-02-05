//==============================================================================//
// Name : ScriptSnowParticle.cpp												// 
// Describe : 	スクリプト:雪パーティクル										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "NativeScript/SnowParticle.h"
#include "NativeScript/ScriptTaskHandle.h"

namespace Fluoresce {

	void ScriptSnowParticle::OnCreate()
	{
		FR_CLIENT_TRACE("ScriptSnowParticle Create");

		m_FallSpeed = 0.5f + (rand() % 50) * 0.01f;
	}

	void ScriptSnowParticle::OnDestroy()
	{
		FR_CLIENT_TRACE("ScriptSnowParticle Destroy");
	}

	void ScriptSnowParticle::OnUpdate(DeltaTime ts)
	{
		auto& translation = GetComponent<TransformComponent>().Translation;

		translation.y += (ts * -m_FallSpeed);

		if (translation.y < -6.0f)
		{
			SetRemove();
		}
	}
}
