//==============================================================================//
// Name : ScriptMoveController2D.h												// 
// Describe : 	�X�N���v�g:2D�R���g���[���[										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	// �X�N���v�g:2D�R���g���[���[
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