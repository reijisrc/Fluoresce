//==============================================================================//
// Name : ScriptTaskHandle.cpp													// 
// Describe : 	スクリプトタスクハンドル										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#include "NativeScript/ScriptTaskHandle.h"

#include "NativeScript/MoveController2D.h"
#include "NativeScript/AutoRotation2D.h"
#include "NativeScript/SnowParticle.h"
#include "NativeScript/SnowParticleManager.h"

namespace Fluoresce {

	namespace Editor
	{
#ifdef FR_RELEASE
		static bool s_EnableDebugInfo = false;
#else
		static bool s_EnableDebugInfo = true;
#endif // FR_RELEASE

		static const std::array<const char*, ScriptID::_ID_Max> s_ScriptName =
		{
			"null",
			"MoveController2D",
			"AutoRotation2D",
			"SnowParticle",
			"SnowParticleManager"
		};

		void ScriptTaskHandle::BindScript(Entity entity)
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			switch (scriptComponent.ScriptID)
			{
			case ScriptID::_MoveController2D:
				scriptComponent.Bind<ScriptMoveController2D>();
				break;
			case ScriptID::_AutoRotation2D:
				scriptComponent.Bind<ScriptAutoRotation2D>();
				break;
			case ScriptID::_SnowParticle:
				scriptComponent.Bind<ScriptSnowParticle>();
				break;
			case ScriptID::_SnowParticleManager:
				scriptComponent.Bind<SnowParticleManager>();
				break;
			default:
				break;
			}

			if (s_EnableDebugInfo)
			{
				UniqueID uid = entity.GetComponent<IDComponent>().ID;
				auto name = s_ScriptName[scriptComponent.ScriptID];
				FR_CLIENT_TRACE("Entity ID: {0} ScriptName: {1}", uid, name);
			}
		}

		const char* ScriptTaskHandle::GetScriptName(uint32 index)
		{
			if (index < ScriptID::_ID_Max)
			{
				return s_ScriptName.at(index);
			}
			return "Undefine Script";
		}

		void ScriptTaskHandle::SetDebugInfoFlag(bool flag)
		{
			s_EnableDebugInfo = false;
		}
	}
}
