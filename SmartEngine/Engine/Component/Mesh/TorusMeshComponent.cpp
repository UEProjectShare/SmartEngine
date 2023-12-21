#include "TorusMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CTorusMeshComponent::CTorusMeshComponent()
{

}

void CTorusMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	const float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);
	const float ThetaValue = XM_2PI / static_cast<float>(InHeightSubdivision);
	for (size_t i = 0; i <= InAxialSubdivision; ++i)
	{
		const float BetaRadian = i* BetaValue;

		//InRadius
		fvector_3d Center(
			InRadius * cosf(BetaRadian),
			0.f,
			InRadius * sinf(BetaRadian));

		for (size_t j = 0; j <= InHeightSubdivision; ++j)
		{
			const float ThetaRadian = j * ThetaValue;

			const float ThetaBetaValueCosCos = cosf(ThetaRadian) * cosf(BetaRadian);
			const float ThetaBetaValueCosSin = cosf(ThetaRadian) * sinf(BetaRadian);
			const float ThetaValueSin = sinf(ThetaRadian);

			//拿到点的位置
			fvector_3d PointPosition(
				Center.x + InSectionRadius * ThetaBetaValueCosCos,
				Center.y + InSectionRadius * ThetaValueSin,
				Center.z + InSectionRadius * ThetaBetaValueCosSin);

			MeshData.VertexData.push_back(
				FVertex(EngineMath::ToFloat3(PointPosition),
				XMFLOAT4(Colors::White)));

			FVertex &InVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

			//求法线
			fvector_3d Normal = PointPosition - Center;
			Normal.normalize();

			//法线赋值
			InVertex.Normal = EngineMath::ToFloat3(Normal);
		}
	}

	for (size_t i = 0; i <= InAxialSubdivision ; ++i)
	{
		for (size_t j = 0; j <InHeightSubdivision; ++j)
		{
			//绘制外圈两个三角形
			DrawQuadrilateral(
				MeshData,//提取绘制信息
				GetQuadrilateralDrawPointTypeA(j, i, InHeightSubdivision));//拿到四个点
		}
	}
}
