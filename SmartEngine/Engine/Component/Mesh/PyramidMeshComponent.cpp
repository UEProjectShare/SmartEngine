#include "PyramidMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPyramidMeshComponent::CPyramidMeshComponent()
{

}

void CPyramidMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	EPyramidNumberSides PyramidNumberSides,
	uint32_t InHeightSubdivide, uint32_t InSize)
{
	const float InRadius = InSize;
	const float InHeight = InSize * 2.f;
	const uint32_t InAxialSubdivision = static_cast<uint32_t>(PyramidNumberSides);

	//半径间隔
	const float RadiusInterval = -InRadius / static_cast<float>(InHeightSubdivide);
	//高度间隔
	const float HeightInterval = InHeight / static_cast<float>(InHeightSubdivide);

	const float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	//构建顶部
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, 0.5f * InHeight, 0.f),
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, 1.f, 0.f)));

	for (uint32_t i = 0; i <= InHeightSubdivide; ++i)
	{
		const float Y = 0.5f * InHeight - HeightInterval * i;
		const float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float c = cosf(j * BetaValue);
			const float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White)));

			FVertex& Vertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			Vertex.UTangent = XMFLOAT3(-s, Y, c);

			const float dr = InRadius;
			XMFLOAT3 bitangent(dr * c, -InHeight, dr * s);

			const XMVECTOR T = XMLoadFloat3(&Vertex.UTangent);
			const XMVECTOR B = XMLoadFloat3(&bitangent);
			const XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&Vertex.Normal, N);

			Vertex.TexCoord.x = static_cast<float>(j) / static_cast<float>(InAxialSubdivision);
			Vertex.TexCoord.y = static_cast<float>(i) / static_cast<float>(InHeightSubdivide);
		}
	}

	//添加中点
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, -0.5f * InHeight, 0.f),
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, -1.f, 0.f)));

	//绘制index模型
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	float BaseIndex = 1;
	const float VertexCircleNum = InAxialSubdivision + 1;
	//绘制腰围
	for (uint32_t i = 0; i <= InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//我们绘制的是四边形
			//三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//添加南极点
	{
		const int i = InHeightSubdivide;

		const float Y = 0.5f * InHeight - HeightInterval * i;
		const float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float c = cosf(j * BetaValue);
			const float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));
		}
	}

	//绘制南极
	const uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + Index);
		MeshData.IndexData.push_back(BaseIndex + Index + 1);
	}
}

void CPyramidMeshComponent::BuildKey(size_t& OutHashKey, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	std::hash<int> IntHash;

	OutHashKey = 7;
	OutHashKey += IntHash._Do_hash((int)InPyramidNumberSidesType);
	OutHashKey += IntHash._Do_hash(InHeightSubdivide);
	OutHashKey += IntHash._Do_hash(InSize);
}
