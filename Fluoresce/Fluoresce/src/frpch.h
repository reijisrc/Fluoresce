//==============================================================================//
// Name : frpch.h																// 
// Describe : 	プリコンパイル済みヘッダー										// 
// Author : Ding Qi																// 
// Create Date : 2022/03/26														// 
// Modify Date : 2022/03/26														// 
//==============================================================================//
#pragma once

#include<iostream>
#include<filesystem>

#include<memory>
#include<utility>
#include<algorithm>
#include<functional>

#include<string>
#include<sstream>
#include<array>
#include<vector>
#include<unordered_map>
#include<unordered_set>

#include "Engine/Core/BaseDefine.h"
#include "Engine/Core/DataDefine.h"
#include "Engine/Core/MathDefine.h"

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"

#ifdef FR_PLATFORM_WINDOWS
#include<Windows.h>
#endif
