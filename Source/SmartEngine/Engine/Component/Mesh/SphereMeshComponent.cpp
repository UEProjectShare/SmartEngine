#include "SphereMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{

}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision,bool bReverse)
{
	MeshData.SectionDescribe.push_back(FMeshSection());
	FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

	float ThetaValue = XM_2PI / InHeightSubdivision;
	float BetaValue = XM_PI / InAxialSubdivision;

	//添加顶部
	MeshData.Data.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, InRadius, 0.f), XMFLOAT4(Colors::Red),
		bReverse ? XMFLOAT3(0.0f, -1.0f, 0.0f) : XMFLOAT3(0.0f, 1.0f, 0.0f), 
		XMFLOAT2(0.5f,0.0f)));

	for (uint32_t i = 1; i < InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;

		for (uint32_t j = 0; j <= InHeightSubdivision; ++j)
		{
			float Theta = j * ThetaValue;

			//球面坐标转为笛卡尔坐标
			MeshData.Data.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//x
					InRadius * cosf(Beta),//y
					InRadius * sinf(Beta) * sinf(Theta)), //z
				XMFLOAT4(Colors::White)));

			int TopIndex = MeshData.Data.VertexData.size() - 1;
			FVertex& InVertex = MeshData.Data.VertexData[TopIndex];

			//存储位置
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);
			XMStoreFloat3(&InVertex.Normal,XMVector3Normalize(Pos));

			//U方向的切线
			InVertex.UTangent.x = -InRadius * sinf(Beta) * sinf(Theta);
			InVertex.UTangent.y = 0.f;
			InVertex.UTangent.z = InRadius * sinf(Beta) * cosf(Theta);

			InVertex.TexCoord.x = Theta / XM_2PI;
			InVertex.TexCoord.y = Beta / XM_PI;

			//存储切线
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));
		}
	}

	//添加底部
	MeshData.Data.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red),
		bReverse ? XMFLOAT3(0.0f, 1.0f, 0.0f) : XMFLOAT3(0.0f, -1.0f, 0.0f),
		XMFLOAT2(0.f,0.5f)));

	//绘制北极
	for (uint32_t Index = 0; Index <= InAxialSubdivision; ++Index)
	{
		if (bReverse)
		{
			MeshData.Data.IndexData.push_back(0);
			MeshData.Data.IndexData.push_back(Index);
			MeshData.Data.IndexData.push_back(Index + 1);
		}
		else
		{
			MeshData.Data.IndexData.push_back(0);
			MeshData.Data.IndexData.push_back(Index + 1);
			MeshData.Data.IndexData.push_back(Index);
		}
	}

	float BaseIndex = 1;
	float VertexCircleNum = InAxialSubdivision + 1;

	//绘制腰围
	for (uint32_t i = 0; i < InHeightSubdivision - 1; ++i)//减去1
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			DrawQuadrilateral(MeshData, 
				GetQuadrilateralDrawPointTypeA(j, i, InAxialSubdivision, 1),
				bReverse);

			////我们绘制的是四边形
			////三角形1
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			////三角形2
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//绘制南极
	uint32_t SouthBaseIndex = MeshData.Data.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		if (bReverse)
		{
			MeshData.Data.IndexData.push_back(BaseIndex + Index);
			MeshData.Data.IndexData.push_back(SouthBaseIndex);
			MeshData.Data.IndexData.push_back(BaseIndex + Index + 1);
		}
		else
		{
			MeshData.Data.IndexData.push_back(SouthBaseIndex);
			MeshData.Data.IndexData.push_back(BaseIndex + Index);
			MeshData.Data.IndexData.push_back(BaseIndex + Index + 1);
		}
	}

	Section.IndexSize = MeshData.Data.IndexData.size();
	Section.VertexSize = MeshData.Data.VertexData.size();

	SpawnDefaultMaterial();
}

void CSphereMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision,bool bReverse)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 8;
	OutHashKey += FloatHash(InRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
	OutHashKey += IntHash._Do_hash(bReverse);
}
