//==============================================================================//
// Name : ScriptMoveController2D.cpp											// 
// Describe : 	スクリプト:2Dコントローラー										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#include "NativeScript/MoveController2D.h"

namespace Fluoresce {

	void ScriptMoveController2D::OnCreate()
	{
		FR_CLIENT_TRACE("ScriptMoveController2D Create");
	}

	void ScriptMoveController2D::OnDestroy()
	{
		FR_CLIENT_TRACE("ScriptMoveController2D Destroy");
	}

	void ScriptMoveController2D::OnUpdate(DeltaTime ts)
	{
		auto& translation = GetComponent<TransformComponent>().Translation;

		if (Input::IsKeyPressed(Key::A))
			translation.x -= m_MoveSpeed * ts;
		if (Input::IsKeyPressed(Key::D))
			translation.x += m_MoveSpeed * ts;
		if (Input::IsKeyPressed(Key::W))
			translation.y += m_MoveSpeed * ts;
		if (Input::IsKeyPressed(Key::S))
			translation.y -= m_MoveSpeed * ts;
	}
}
