//==============================================================================//
// Name : MathDefine.h															// 
// Describe :	数学データ定義													// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#pragma once

#include <glm/glm.hpp>

namespace Fluoresce {

	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;

	typedef glm::mat3x3 Mat3;
	typedef glm::mat4x4 Mat4;

	constexpr Vec3 VectorZero = Vec3(0.0f, 0.0f, 0.0f);
	constexpr Vec3 VectorOne = Vec3(1.0f, 1.0f, 1.0f);
	constexpr Vec3 AxisX = Vec3(1.0f, 0.0f, 0.0f);
	constexpr Vec3 AxisY = Vec3(0.0f, 1.0f, 0.0f);
	constexpr Vec3 AxisZ = Vec3(0.0f, 0.0f, 1.0f);
}
