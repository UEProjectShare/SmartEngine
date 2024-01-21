#pragma once
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex
{
	FVertex();
	
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor);
	
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT2& InTexCoord = XMFLOAT2(0.f, 0.f));

	XMFLOAT3 Position;
	
	XMFLOAT4 Color;
	
	XMFLOAT3 Normal;
	
	XMFLOAT3 UTangent;
	
	XMFLOAT2 TexCoord;//纹理坐标
};

struct FMeshRenderingData
{
	vector<FVertex> VertexData;
	
	vector<uint16_t> IndexData;
	
	UINT GetVertexSizeInBytes() const { return VertexData.size() * sizeof(FVertex); }
	
	UINT GetIndexSizeInBytes() const { return IndexData.size() * sizeof(uint16_t); }
};

//Pyramid的边数
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	
	Pyramid_4 ,
	
	Pyramid_5 ,
};