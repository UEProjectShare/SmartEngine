#include "TorusMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CTorusMeshComponent::CTorusMeshComponent()
{

}

void CTorusMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	MeshData.SectionDescribe.push_back(FMeshSection());
	FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

	float BetaValue = XM_2PI / (float)InAxialSubdivision;
	float ThetaValue = XM_2PI / (float)InHeightSubdivision;
	for (size_t i = 0; i <= InAxialSubdivision; ++i)
	{
		float BetaRadian = i* BetaValue;

		//InRadius
		fvector_3d Center(
			InRadius * cosf(BetaRadian),
			0.f,
			InRadius * sinf(BetaRadian));

		for (size_t j = 0; j <= InHeightSubdivision; ++j)
		{
			float ThetaRadian = j * ThetaValue;

			float ThetaBetaValueCosCos = cosf(ThetaRadian) * cosf(BetaRadian);
			float ThetaBetaValueCosSin = cosf(ThetaRadian) * sinf(BetaRadian);
			float ThetaValueSin = sinf(ThetaRadian);

			//拿到点的位置
			fvector_3d PointPosition(
				Center.x + InSectionRadius * ThetaBetaValueCosCos,
				Center.y + InSectionRadius * ThetaValueSin,
				Center.z + InSectionRadius * ThetaBetaValueCosSin);

			MeshData.Data.VertexData.push_back(
				FVertex(EngineMath::ToFloat3(PointPosition),
				XMFLOAT4(Colors::White)));

			FVertex &InVertex = MeshData.Data.VertexData[MeshData.Data.VertexData.size() - 1];

			//求法线
			fvector_3d Normal = PointPosition - Center;
			Normal.normalize();

			//法线赋值
			InVertex.Normal = EngineMath::ToFloat3(Normal);

			//展UV
			InVertex.TexCoord.x = (float)j / (float)InHeightSubdivision;
			InVertex.TexCoord.y = (float)i / (float)InAxialSubdivision;

			//InVertex.UTangent.x = tan(BetaRadian) * InRadius;
			//InVertex.UTangent.y = tan(ThetaRadian) * InSectionRadius;
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

	Section.IndexSize = MeshData.Data.IndexData.size();
	Section.VertexSize = MeshData.Data.VertexData.size();
}

void CTorusMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 9;
	OutHashKey += FloatHash(InRadius);
	OutHashKey += FloatHash(InSectionRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
