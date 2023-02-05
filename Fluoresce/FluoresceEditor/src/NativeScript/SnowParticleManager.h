//==============================================================================//
// Name : SnowParticleManager.h													// 
// Describe : 	スクリプト:雪パーティクルマネージャー							// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// スクリプト:雪パーティクルマネージャー
	class SnowParticleManager : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(DeltaTime ts) override;

	private:
		void CreateSnowEntity();
	private:
		float32 m_Timer = 0.0f;
	};
}