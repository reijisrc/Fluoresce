//==============================================================================//
// Name : ScriptMoveController2D.h												// 
// Describe : 	スクリプト:2Dコントローラー										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// スクリプト:2Dコントローラー
	class ScriptMoveController2D : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(DeltaTime ts) override;
	private:
		float32 m_MoveSpeed = 5.0f;
	};
}