#include "SphereMesh.h"
#include "Core/MeshType.h"

void GSphereMesh::Init()
{
	Super::Init();


}

void GSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);


}

void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GSphereMesh::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	const float ThetaValue = XM_2PI / InHeightSubdivision;
	const float BetaValue = XM_PI / InAxialSubdivision;

	//添加顶部
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, InRadius,0.f), XMFLOAT4(Colors::Red),
		XMFLOAT3(0.0f, 1.0f, 0.0f)));

	for (uint32_t i = 1; i < InAxialSubdivision; ++i)
	{
		const float Beta = i * BetaValue;

		for (uint32_t j = 0; j <= InHeightSubdivision; ++j)
		{
			const float Theta = j * ThetaValue;

			//球面坐标转为笛卡尔坐标
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//x
					InRadius * cosf(Beta),//y
					InRadius * sinf(Beta) * sinf(Theta)), //z
				XMFLOAT4(Colors::White)));

			const int TopIndex = MeshData.VertexData.size() - 1;
			FVertex& InVertex = MeshData.VertexData[TopIndex];

			//存储位置
			const XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));

			//U方向的切线
			//https://www.cnblogs.com/flytrace/p/3387748.html
			InVertex.UTangent.x = -InRadius * sinf(Beta) * sinf(Theta);
			InVertex.UTangent.y = 0.f;
			InVertex.UTangent.z = InRadius * sinf(Beta) * cosf(Theta);

			//存储切线
			const XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));
		}
	}

	//添加底部
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red),
		XMFLOAT3(0.0f, -1.0f, 0.0f)));

	//绘制北极
	for (uint32_t Index = 0; Index <= InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(Index +1);
		MeshData.IndexData.push_back(Index);
	}

	float BaseIndex = 1;
	const float VertexCircleNum = InAxialSubdivision + 1;
	//绘制腰围
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
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
