#include "BoxMesh.h"
#include "Core/MeshType.h"

void FBoxMesh::Init()
{
	Super::Init();


}

void FBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);


}

void FBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

FBoxMesh* FBoxMesh::CreateMesh()
{
	FMeshRenderingData MeshData;

	//构建我们的顶点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::White)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT4(Colors::AliceBlue)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(1.f, 1.f, 0.f), XMFLOAT4(Colors::Aqua)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT4(Colors::Aquamarine)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 0.f, 1.f), XMFLOAT4(Colors::Bisque)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 1.f, 1.f), XMFLOAT4(Colors::Blue)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(Colors::Chocolate)));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(1.f, 0.f, 1.f), XMFLOAT4(Colors::Chocolate)));

	//构建我们的索引
	//前
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

	//后
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

	//左
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

	//右
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

	//上
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

	//下
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);

	FBoxMesh* BoxMesh = new FBoxMesh;
	BoxMesh->BuildMesh(&MeshData);

	BoxMesh->Init();

	return BoxMesh;
}
