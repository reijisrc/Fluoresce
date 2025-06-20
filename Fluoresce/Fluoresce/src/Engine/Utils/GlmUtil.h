//==============================================================================//
// Name : GlmUtil.h																// 
// Describe :	GLM�ėp�@�\														// 
// Author : Ding Qi																// 
// Create Date : 2022/04/23														// 
// Modify Date : 2022/04/23														// 
//==============================================================================//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Fluoresce {

	// TRS����s��w�ϊ�
	static void ComposeTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, glm::mat4& outTransform)
	{
		glm::quat orientation = glm::quat(rotation);
		glm::mat4 rotationmat = glm::toMat4(orientation);

		outTransform = glm::translate(glm::mat4(1.0f), translation)
			* rotationmat
			* glm::scale(glm::mat4(1.0f), scale);
	}

	// �s�񂩂�TRS�w�o��
	static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
	{
		// From glm::decompose in matrix_decompose.inl
		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		outTranslation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		outScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		outScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		outScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		vec3 Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		outRotation.y = asin(-Row[0][2]);
		if (cos(outRotation.y) != 0) {
			outRotation.x = atan2(Row[1][2], Row[2][2]);
			outRotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			outRotation.x = atan2(-Row[2][0], Row[1][1]);
			outRotation.z = 0;
		}

		return true;
	}
}
