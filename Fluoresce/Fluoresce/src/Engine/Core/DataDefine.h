//==============================================================================//
// Name : DataDefine.h															// 
// Describe :	データ定義														// 
// Author : Ding Qi																// 
// Create Date : 2022/03/22														// 
// Modify Date : 2022/08/13														// 
//==============================================================================//
#pragma once

#include <memory>

namespace Fluoresce {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	// データ転送用汎用Vec4 
	struct FrVec4
	{
		union
		{
			float value[4];
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				float r, g, b, a;
			};
		};

		FrVec4() = default;

		FrVec4(float v)
		{
			x = v;
			y = v;
			z = v;
			w = v;
		}

		FrVec4(float vx, float vy, float vz, float vw)
		{
			x = vx;
			y = vy;
			z = vz;
			w = vw;
		};

		FrVec4 operator +() const
		{
			return *this;
		}

		FrVec4 operator -() const
		{
			return{ -x, -y, -z, -w };
		}

		FrVec4 operator +(const FrVec4& other) const
		{
			return FrVec4{ x + other.x, y + other.y, z + other.z, w + other.w };
		}

		FrVec4 operator -(const FrVec4& other) const
		{
			return FrVec4{ x - other.x, y - other.y, z - other.z, w - other.w };
		}

		FrVec4 operator *(float mult) const
		{
			return FrVec4{ x * mult, y * mult, z * mult, w * mult };
		}

		FrVec4 operator +=(const FrVec4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		FrVec4 operator -=(const FrVec4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		const float* GetConstPtr() const
		{
			return &value[0];
		}

		float* GetPtr()
		{
			return &value[0];
		}
	};

	typedef signed char sint8;
	typedef unsigned char uint8;
	typedef int sint32;
	typedef unsigned int  uint32;
	typedef float float32;
	typedef long long sint64;
	typedef unsigned long long uint64;
}
