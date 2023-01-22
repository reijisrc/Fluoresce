//==============================================================================//
// Name : Camera.h																// 
// Describe :	カメラ															// 
// Author : Ding Qi																// 
// Create Date : 2022/08/13														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

namespace Fluoresce {

	// カメラ(プロジェクションのみ)
	class Camera
	{
	public:
		Camera() = default;
		Camera(const Mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		void SetProjection(const Mat4& projection) { m_Projection = projection; }
		const Mat4& GetProjection() const { return m_Projection; }
	protected:
		Mat4 m_Projection = Mat4(1.0f);
	};

}