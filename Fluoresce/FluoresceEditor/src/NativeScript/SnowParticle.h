//==============================================================================//
// Name : ScriptSnowParticle.h													// 
// Describe : 	スクリプト:雪パーティクル										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// スクリプト:2Dコントローラー
	class ScriptSnowParticle : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(DeltaTime ts) override;
	private:
		float32 m_FallSpeed = 0.0f;
	};
}