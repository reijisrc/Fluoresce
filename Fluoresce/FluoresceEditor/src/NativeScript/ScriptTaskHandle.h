//==============================================================================//
// Name : ScriptTaskHandle.h													// 
// Describe : 	スクリプトタスクハンドル										// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/02/06														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		enum ScriptID : uint32
		{
			_Null = 0,
			_MoveController2D,
			_AutoRotation2D,
			_SnowParticle,
			_SnowParticleManager,
			_ID_Max
		};

		// スクリプトタスクハンドル
		class ScriptTaskHandle
		{
		public:
			static void BindScript(Entity entity);

			static const char* GetScriptName(uint32 index);

			static void SetDebugInfoFlag(bool flag);
		};
	}
}
