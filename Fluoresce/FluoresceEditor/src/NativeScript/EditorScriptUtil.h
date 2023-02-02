//==============================================================================//
// Name : EditorScriptUtil.h													// 
// Describe : 	エディタースクリプト汎用機能									// 
// Author : Ding Qi																// 
// Create Date : 2023/01/26														// 
// Modify Date : 2023/01/26														// 
//==============================================================================//
#pragma once

#include "Fluoresce.h"

namespace Fluoresce {

	namespace Editor
	{
		enum EditorScriptID : uint32
		{
			_Null = 0,
			_MoveController2D,
			_AutoRotation2D,
			_ID_Max
		};

		// エディタースクリプト汎用機能
		class EditorScriptUtil
		{
		public:
			using NameArray = std::array<const char*, EditorScriptID::_ID_Max>;

			static void BindScript(Entity entity);

			static const char* GetScriptName(uint32 index);
		};
	}
}
