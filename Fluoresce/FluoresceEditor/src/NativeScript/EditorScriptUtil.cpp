//==============================================================================//
// Name : EditorScriptUtil.cpp													// 
// Describe : 	エディタースクリプト汎用機能									// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#include "NativeScript/EditorScriptUtil.h"

#include "NativeScript/MoveController2D.h"
#include "NativeScript/AutoRotation2D.h"

namespace Fluoresce {

	namespace Editor
	{
		static const EditorScriptUtil::NameArray s_ScriptName =
		{
			"null",
			"MoveController2D",
			"AutoRotation2D"
		};

		void EditorScriptUtil::BindScript(Entity entity)
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			switch (scriptComponent.ScriptID)
			{
			case EditorScriptID::_MoveController2D:
				scriptComponent.Bind<ScriptMoveController2D>();
				break;
			case EditorScriptID::_AutoRotation2D:
				scriptComponent.Bind<ScriptAutoRotation2D>();
				break;
			default:
				break;
			}
		}

		const char* EditorScriptUtil::GetScriptName(uint32 index)
		{
			if (index < EditorScriptID::_ID_Max)
			{
				return s_ScriptName.at(index);
			}
			return "Undefine Script";
		}
	}
}
