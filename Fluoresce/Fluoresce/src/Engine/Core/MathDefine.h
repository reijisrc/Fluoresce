//==============================================================================//
// Name : MathDefine.h															// 
// Describe :	数学データ定義													// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/10/15														// 
//==============================================================================//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Fluoresce {

	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;

	typedef glm::mat3x3 Mat3;
	typedef glm::mat4x4 Mat4;

	namespace Float
	{
		constexpr float Min = 1.175494e-38f;
		constexpr float Max = 3.402823e+38f;
		constexpr float Epsilon = 1.19209e-07f;
		constexpr float PI = 3.1415926536f;
	};

	namespace Vector {
		constexpr Vec3 AxisX = Vec3(1.0f, 0.0f, 0.0f);
		constexpr Vec3 AxisY = Vec3(0.0f, 1.0f, 0.0f);
		constexpr Vec3 AxisZ = Vec3(0.0f, 0.0f, 1.0f);
	}

}
