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

	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, InRadius,0.f),XMFLOAT4(Colors::Red)));

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

			const XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexData[TopIndex].Position);
			XMStoreFloat3(&MeshData.VertexData[TopIndex].Normal,XMVector3Normalize(Pos));
		}
	}

	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red)));

	//绘制北极
	for (uint32_t Index = 1; Index <= InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back( Index >InAxialSubdivision ? (Index + 1) % InAxialSubdivision : Index + 1);
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
