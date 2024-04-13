#include "BoxMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CBoxMeshComponent::CBoxMeshComponent()
{

}

void CBoxMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
	MeshData.SectionDescribe.push_back(FMeshSection());
	FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;
	
	//前面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White),XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2( 0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White),XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2( 0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White),XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2( 1.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White),XMFLOAT3(0.0f, 0.0f, -1.0f),XMFLOAT2( 1.0f, 1.0f)));

	//后面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White),  XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)));

	//上面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)));

	//右面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, -1.0f, 0.0f),XMFLOAT2(1.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)));
					   
	//左面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)));

	//右面点
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, -CDepth),XMFLOAT4(Colors::White), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, +CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)));
	MeshData.Data.VertexData.push_back(FVertex(XMFLOAT3(+CWidth, -CHeight, +CDepth),XMFLOAT4(Colors::White), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)));

	//绘制模型
	//前面
	MeshData.Data.IndexData.push_back(0); MeshData.Data.IndexData.push_back(1); MeshData.Data.IndexData.push_back(2);
	MeshData.Data.IndexData.push_back(0); MeshData.Data.IndexData.push_back(2); MeshData.Data.IndexData.push_back(3);

	//后面
	MeshData.Data.IndexData.push_back(4); MeshData.Data.IndexData.push_back(5); MeshData.Data.IndexData.push_back(6);
	MeshData.Data.IndexData.push_back(4); MeshData.Data.IndexData.push_back(6); MeshData.Data.IndexData.push_back(7);

	//顶部
	MeshData.Data.IndexData.push_back(8); MeshData.Data.IndexData.push_back(9); MeshData.Data.IndexData.push_back(10);
	MeshData.Data.IndexData.push_back(8); MeshData.Data.IndexData.push_back(10); MeshData.Data.IndexData.push_back(11);

	//下面
	MeshData.Data.IndexData.push_back(12); MeshData.Data.IndexData.push_back(13); MeshData.Data.IndexData.push_back(14);
	MeshData.Data.IndexData.push_back(12); MeshData.Data.IndexData.push_back(14); MeshData.Data.IndexData.push_back(15);

	//左面
	MeshData.Data.IndexData.push_back(16); MeshData.Data.IndexData.push_back(17); MeshData.Data.IndexData.push_back(18);
	MeshData.Data.IndexData.push_back(16); MeshData.Data.IndexData.push_back(18); MeshData.Data.IndexData.push_back(19);

	//右面
	MeshData.Data.IndexData.push_back(20); MeshData.Data.IndexData.push_back(21); MeshData.Data.IndexData.push_back(22);
	MeshData.Data.IndexData.push_back(20); MeshData.Data.IndexData.push_back(22); MeshData.Data.IndexData.push_back(23);

	Section.IndexSize = MeshData.Data.IndexData.size();
	Section.VertexSize = MeshData.Data.VertexData.size();
}

void CBoxMeshComponent::BuildKey(size_t& OutHashKey, float InHeight, float InWidth, float InDepth)
{
	const std::hash<float> FloatHash;

	OutHashKey = 1;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InWidth);
	OutHashKey += FloatHash(InDepth);
}
