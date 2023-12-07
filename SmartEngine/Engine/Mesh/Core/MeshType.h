#pragma once
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex
{
	FVertex(const XMFLOAT3 &InPos,const XMFLOAT4 &InColor);

	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
};

struct FMeshRenderingData
{
	vector<FVertex> VertexData;
	vector<uint16_t> IndexData;
};
