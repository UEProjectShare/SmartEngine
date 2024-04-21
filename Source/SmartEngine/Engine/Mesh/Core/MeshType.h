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

struct FSkinnedVertex : public FVertex
{
	FSkinnedVertex(){}

};

template<class T>
struct FMeshData
{
	vector<T> VertexData;
	
	vector<uint16_t> IndexData;

	bool IsVaild() const { return VertexData.size() != 0 && IndexData.size() != 0; }
	
	UINT GetVertexSizeInBytes() { return VertexData.size() * sizeof(T); }
	
	UINT GetIndexSizeInBytes() const { return IndexData.size() * sizeof(uint16_t); }
};

struct FMeshSection
{
	FMeshSection();

	UINT IndexSize;
	
	UINT VertexSize;

	int MaterialIndex;
};

template<class T>
struct FRenderContent
{
	FRenderContent()
	{}

	std::vector<FMeshSection> SectionDescribe;
	
	FMeshData<T> Data;
};

//Pyramid的边数
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	Pyramid_4,
	Pyramid_5,
};

typedef FMeshData<FVertex> FVertexMeshData;

typedef FMeshData<FSkinnedVertex> FSkinnedVertexMeshData;

typedef FRenderContent<FVertex> FMeshRenderingData;

typedef FRenderContent<FSkinnedVertex> FSkinnedMeshRenderingData;