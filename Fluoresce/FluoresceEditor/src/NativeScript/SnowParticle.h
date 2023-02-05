//==============================================================================//
// Name : ScriptSnowParticle.h													// 
// Describe : 	�X�N���v�g:��p�[�e�B�N��										// 
// Author : Ding Qi																// 
// Create Date : 2023/02/06														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// �X�N���v�g:2D�R���g���[���[
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