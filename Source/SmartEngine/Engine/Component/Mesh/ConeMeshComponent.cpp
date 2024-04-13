#include "ConeMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CConeMeshComponent::CConeMeshComponent()
{

}

void CConeMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData,
	float InRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	MeshData.SectionDescribe.push_back(FMeshSection());
	FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

	//半径间隔
	float RadiusInterval = -InRadius / (float)InHeightSubdivision;
	//高度间隔
	float HeightInterval = InHeight / (float)InHeightSubdivision;

	float BetaValue = XM_2PI / (float)InAxialSubdivision;

	//构建顶部
	MeshData.Data.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, 0.5f * InHeight, 0.f), 
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, 1.f, 0.f)));

	//uint32_t Index = MeshData.VertexData.size();

	for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i;
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float c = cosf(j * BetaValue);
			float s = sinf(j * BetaValue);
			MeshData.Data.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White)));

			FVertex &Vertex = MeshData.Data.VertexData[MeshData.Data.VertexData.size() - 1];
			Vertex.UTangent = XMFLOAT3(-s, Y, c);
			
			float dr = InRadius;
			XMFLOAT3 bitangent(dr * c, -InHeight, dr * s);
			
			XMVECTOR T = XMLoadFloat3(&Vertex.UTangent);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&Vertex.Normal, N);
		}
	}

	//添加中点
	MeshData.Data.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, -0.5f * InHeight, 0.f),
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, -1.f, 0.f)));

	//绘制index模型
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		MeshData.Data.IndexData.push_back(0);
		MeshData.Data.IndexData.push_back(i + 1);
		MeshData.Data.IndexData.push_back(i);
	}

	float BaseIndex = 1;
	float VertexCircleNum = InAxialSubdivision + 1;
	//绘制腰围
	for (uint32_t i = 0; i <= InHeightSubdivision - 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//我们绘制的是四边形
			//三角形1
			MeshData.Data.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.Data.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.Data.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.Data.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.Data.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.Data.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//添加南极点
	{
		int i = InHeightSubdivision;

		float Y = 0.5f * InHeight - HeightInterval * i;
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float c = cosf(j * BetaValue);
			float s = sinf(j * BetaValue);
			MeshData.Data.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White), XMFLOAT3(0.f,-1.f,0.f)));
		}
	}

	//绘制南极
	uint32_t SouthBaseIndex = MeshData.Data.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		MeshData.Data.IndexData.push_back(SouthBaseIndex);
		MeshData.Data.IndexData.push_back(BaseIndex + Index);
		MeshData.Data.IndexData.push_back(BaseIndex + Index + 1);
	}

	Section.IndexSize = MeshData.Data.IndexData.size();
	Section.VertexSize = MeshData.Data.VertexData.size();
}

void CConeMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	const std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 2;
	OutHashKey += FloatHash(InRadius);
	OutHashKey += FloatHash(InHeight);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
