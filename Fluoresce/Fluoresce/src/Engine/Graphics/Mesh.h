//==============================================================================//
// Name : Mesh.h																// 
// Describe :	メッシュ														// 
// Author : Ding Qi																// 
// Create Date : 2023/02/12														// 
// Modify Date : 2023/02/12														// 
//==============================================================================//
#pragma once

#include "GPUDataDefine.h"

namespace Fluoresce {

	// 標準頂点データ構造
	struct StandardVertex
	{
		Vec3 Position;
		Vec3 Normal;
		Vec2 Texcoord;
		Vec3 Tangent;
	};

	// 標準頂点データ
	struct StandardVertexData
	{
		std::vector<Vec3> Positions;
		std::vector<Vec3> Normals;
		std::vector<Vec2> Texcoords;
		std::vector<Vec3> Tangents;
		std::vector<uint32> Indices;
	};


	// メッシュ
	class Mesh
	{
	public:
		virtual ~Mesh() {}
	};
}