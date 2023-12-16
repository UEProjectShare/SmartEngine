#pragma once
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex
{
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor);
	
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3 &Normal);

	XMFLOAT3 Position;
	
	XMFLOAT4 Color;
	
	XMFLOAT3 Normal;
	
	XMFLOAT3 UTangent;
};

struct FMeshRenderingData
{
	vector<FVertex> VertexData;
	
	vector<uint16_t> IndexData;
	
	UINT GetVertexSizeInBytes() const { return VertexData.size() * sizeof(FVertex); }
	
	UINT GetIndexSizeInBytes() const { return IndexData.size() * sizeof(uint16_t); }
};
