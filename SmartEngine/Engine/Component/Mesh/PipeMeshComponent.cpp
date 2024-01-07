#include "PipeMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

struct FPipeMeshUVCalculationInfo
{
	fvector_2d OuterStart;
	fvector_2d OuterOffset;
	fvector_2d InnerStart;
	fvector_2d InnerOffset;
	fvector_2d TopRadiusStart;
	fvector_2d BottomRadiusStart;
};

CPipeMeshComponent::CPipeMeshComponent()
{

}

void CPipeMeshComponent::BuildRadiusPoint(
	FMeshRenderingData& MeshData,
	float HeightInterval,
	float InTopRadius,
	float InBottomRadius,
	float BetaValue,
	float RadiusInterval,
	float InHeight,
	float InThickness,
	uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	//����UV�ı�ֵ
	//FPipeMeshUVCalculationInfo UVCalculationInfo;
	//{
	//	//���õ�����߶�
	//	float OuterHeight = InHeight;
	//	float InnerHeight = InHeight;
	//	if (InTopRadius != InBottomRadius)
	//	{
	//		//����������Ǹ��ⲿ�ֵ�б�泤��
	//		float ExcessPart = abs(InTopRadius - InBottomRadius);
	//		OuterHeight = sqrt(pow(ExcessPart, 2) + pow(InHeight, 2));
	//	}

	//	//ֱ��
	//	float MaxDiameter = max(InTopRadius, InBottomRadius) *2.f;

	//	//�ܳ��� Ŀǰ��λy��
	//	float TotalLength = OuterHeight + InnerHeight + MaxDiameter;

	//	//����Ȧ��ʼ
	//	UVCalculationInfo.OuterStart.y = 0.f;
	//	UVCalculationInfo.OuterStart.x = 0.f;
	//	UVCalculationInfo.InnerStart.y = OuterHeight / TotalLength;
	//	UVCalculationInfo.InnerStart.x = 0.f;

	//	//����뾶�Ͷ��뾶
	//	float RadiusHeight = (OuterHeight + InnerHeight) / TotalLength;
	//	if (InTopRadius > InBottomRadius)
	//	{
	//		UVCalculationInfo.TopRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.TopRadiusStart.x = 0.f;

	//		UVCalculationInfo.BottomRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.BottomRadiusStart.x = MaxDiameter / TotalLength;
	//	}
	//	else if (InTopRadius < InBottomRadius)
	//	{
	//		UVCalculationInfo.BottomRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.BottomRadiusStart.x = 0.f;

	//		UVCalculationInfo.TopRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.TopRadiusStart.x = MaxDiameter / TotalLength;
	//	}
	//	else
	//	{
	//		UVCalculationInfo.TopRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.TopRadiusStart.x = 0.f;
	//		UVCalculationInfo.BottomRadiusStart.y = RadiusHeight;
	//		UVCalculationInfo.BottomRadiusStart.x = MaxDiameter / TotalLength;
	//	}
	//}

	//����������Ȧ
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i;

		//���������Ȧ�뾶
		float OuterRadius = InTopRadius + i * RadiusInterval;
		const float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);
		
		for (size_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float BetaValueCos = cosf(j * BetaValue);
			const float BetaValueSin = sinf(j * BetaValue);

			//��Ȧ��
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					OuterRadius * BetaValueCos,//x
					Y,//y
					OuterRadius * BetaValueSin), //z
				XMFLOAT4(Colors::White)));

			{
				FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

				float D = InBottomRadius - InTopRadius;
				XMFLOAT3 Bitangent(D * BetaValueCos, -InHeight, D * BetaValueSin);

				const XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
				const XMVECTOR B = XMLoadFloat3(&Bitangent);
				const XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&MyVertex.Normal, N);

				MyVertex.TexCoord.x = static_cast<float>(j) / static_cast<float>(InAxialSubdivision);
				MyVertex.TexCoord.y = static_cast<float>(i) / static_cast<float>(InHeightSubdivision);
			}

			//������Ȧ
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InnerRadius * BetaValueCos,//x
					Y,//y
					InnerRadius * BetaValueSin), //z
				XMFLOAT4(Colors::White)));

			{
				FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

				const float D = InBottomRadius - InTopRadius;
				XMFLOAT3 Bitangent(D * BetaValueCos, -InHeight, D * BetaValueSin);

				const XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
				const XMVECTOR B = XMLoadFloat3(&Bitangent);
				const XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&MyVertex.Normal, N);

				MyVertex.TexCoord.x = static_cast<float>(j) / static_cast<float>(InAxialSubdivision);
				MyVertex.TexCoord.y = static_cast<float>(i) / static_cast<float>(InHeightSubdivision);
			}
		}
	}
}

void CPipeMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	float InTopRadius,
	float InBottomRadius, 
	float InHeight,
	float InThickness,
	uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	//�뾶���
	const float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;
	//�߶ȼ��
	const float HeightInterval = InHeight / InHeightSubdivision;

	const float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	BuildRadiusPoint(
		MeshData,
		HeightInterval,
		InTopRadius,
		InBottomRadius,
		BetaValue,
		RadiusInterval,
		InHeight,
		InThickness, 
		InAxialSubdivision,
		InHeightSubdivision);

	const float VertexCircleNum = InAxialSubdivision;

	//������Χ
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			const int OuterStartPoint = j * 2;
			const int InnerStartPoint = j * 2 + 1;

			const int VC = VertexCircleNum * 2 ;

			//����Ȧ��
			DrawQuadrilateral(
				MeshData,//��ȡ������Ϣ
				GetQuadrilateralDrawPointTypeA(OuterStartPoint, i, VC, 2));//�õ�Ȧ���ĸ���
			
			//����Ȧ��
			DrawQuadrilateral(
				MeshData,//��ȡ������Ϣ
				GetQuadrilateralDrawPointTypeA(InnerStartPoint, i, VC, 2), true);//�õ�Ȧ���ĸ���																		
		}
	}

	//��������
	if (1)
	{
		for (uint32_t i = 0; i < 1; ++i)
		{
			const float Y = 0.5f * InHeight - HeightInterval * i;

			//���������Ȧ�뾶
			float OuterRadius = InTopRadius + i * RadiusInterval;
			const float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);

			for (size_t j = 0; j <= InAxialSubdivision; ++j)
			{
				const float BetaValueCos = cosf(j * BetaValue);
				const float BetaValueSin = sinf(j * BetaValue);

				//��Ȧ��
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						OuterRadius * BetaValueCos,//x
						Y,//y
						OuterRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f,1.f,0.f)));

				FVertex& InOuterVertex = MeshData.VertexData[MeshData.VertexData.size()-1];
				
				InOuterVertex.TexCoord.x =(BetaValueCos * 0.5f) + 0.5f;
				InOuterVertex.TexCoord.y =(BetaValueSin * 0.5f) + 0.5f;

				//������Ȧ
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						InnerRadius * BetaValueCos,//x
						Y,//y
						InnerRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.f)));

				FVertex& InnerVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				const float ExtraRadiusRatio = (OuterRadius - InnerRadius) / InTopRadius;

				InnerVertex.TexCoord.x = ((1.f - ExtraRadiusRatio)*BetaValueCos * 0.5f)+0.5f;
				InnerVertex.TexCoord.y = ((1.f - ExtraRadiusRatio)*BetaValueSin * 0.5f)+0.5f;
			}
		}
		const int BaseIndex = (InHeightSubdivision + 2) * (InAxialSubdivision ) * 2 + 2;
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			const int OuterStartPoint = j * 2;
			const int InnerStartPoint = j * 2 + 1;

			fvector_4id DrawPoint= {
						BaseIndex+InnerStartPoint,
						BaseIndex+OuterStartPoint,
						BaseIndex+OuterStartPoint + 2,
						BaseIndex+InnerStartPoint + 2 };

			DrawQuadrilateral(MeshData,DrawPoint);
		}
	}

	//�����ײ�
	if (1)
	{
		{
			float Y = 0.5f * InHeight - HeightInterval * InHeightSubdivision;

			//���������Ȧ�뾶
			const float OuterRadius = InTopRadius + InHeightSubdivision * RadiusInterval;
			const float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);

			for (size_t j = 0; j <= InAxialSubdivision; ++j)
			{
				const float BetaValueCos = cosf(j * BetaValue);
				const float BetaValueSin = sinf(j * BetaValue);

				//��Ȧ��
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						OuterRadius * BetaValueCos,//x
						Y,//y
						OuterRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));

				FVertex& InOuterVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
				InOuterVertex.TexCoord.x = (BetaValueCos * 0.5f) + 0.5f;
				InOuterVertex.TexCoord.y = (BetaValueSin * 0.5f) + 0.5f;

				//������Ȧ
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						InnerRadius * BetaValueCos,//x
						Y,//y
						InnerRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));

				FVertex& InnerVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				const float ExtraRadiusRatio = (OuterRadius - InnerRadius) / InTopRadius;

				InnerVertex.TexCoord.x = ((1.f - ExtraRadiusRatio) * BetaValueCos * 0.5f) + 0.5f;
				InnerVertex.TexCoord.y = ((1.f - ExtraRadiusRatio) * BetaValueSin * 0.5f) + 0.5f;
			}
		}

		const int BaseIndex = (InHeightSubdivision + 3) * InAxialSubdivision * 2 + 4;
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			const int OuterStartPoint = j * 2;
			const int InnerStartPoint = j * 2 + 1;

			fvector_4id DrawPoint = {
						 BaseIndex+InnerStartPoint,
						 BaseIndex+OuterStartPoint,
						 BaseIndex+OuterStartPoint + 2,
						 BaseIndex+InnerStartPoint + 2 };

			DrawQuadrilateral(MeshData, DrawPoint,true);
		}
	}
}

void CPipeMeshComponent::BuildKey(size_t& OutHashKey, float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	const std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 5;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InBottomRadius);
	OutHashKey += FloatHash(InTopRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
