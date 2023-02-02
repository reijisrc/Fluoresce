//==============================================================================//
// Name : ScriptAutoRotation2D.h												// 
// Describe : 	スクリプト:自動回転												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// スクリプト:自動回転
	class ScriptAutoRotation2D : public ScriptableEntity
	{
	public:
		virtual void OnCreate() override;

		virtual void OnDestroy() override;

		virtual void OnUpdate(DeltaTime ts) override;
	private:
		float32 m_Rotate = 0.0f;
		float32 m_AngularVelocity = 4.0f;
		float32 m_Radius = 4.0f;
	};
}