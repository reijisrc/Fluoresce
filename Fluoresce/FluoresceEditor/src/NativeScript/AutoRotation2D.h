//==============================================================================//
// Name : ScriptAutoRotation2D.h												// 
// Describe : 	�X�N���v�g:������]												// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// �X�N���v�g:������]
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