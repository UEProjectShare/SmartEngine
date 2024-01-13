#include "CylinderMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCylinderMeshComponent::CCylinderMeshComponent()
{

}

void CCylinderMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//�뾶���
	const float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;
	//�߶ȼ��
	const float HeightInterval = InHeight / InHeightSubdivision;

	const float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		const float Y = 0.5f * InHeight - HeightInterval * i;
		const float Radius = InTopRadius + i * RadiusInterval;
		for (size_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float BetaValueCos = cosf(j * BetaValue);
			const float BetaValueSin = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * BetaValueCos,//x
					Y,//y
					Radius * BetaValueSin), //z
				XMFLOAT4(Colors::White)));

			FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

			MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

			const float dr = InBottomRadius-InTopRadius;
			XMFLOAT3 Bitangent(dr * BetaValueCos, -InHeight, dr * BetaValueSin);

			const XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
			const XMVECTOR B = XMLoadFloat3(&Bitangent);
			const XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&MyVertex.Normal, N);

			//չUV
			MyVertex.TexCoord.x = static_cast<float>(j) / static_cast<float>(InHeightSubdivision);
			MyVertex.TexCoord.y = static_cast<float>(i) / static_cast<float>(InAxialSubdivision);
		}
	}

	const float VertexCircleNum = InAxialSubdivision;

	//������Χ
	for (uint32_t i = 0; i < InHeightSubdivision + 1 ; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//���ǻ��Ƶ����ı���
			// 
			// ����Զ�������
			//������1
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1 )* VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j+1);

			////������2
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);

			// ���߳����Լ�
			//������1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);//C
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j);//B
			MeshData.IndexData.push_back(i * VertexCircleNum + j);//A

			//������2
			MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);//D
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);//C
			MeshData.IndexData.push_back(i * VertexCircleNum + j);//A
		}
	}

	//��������
	if (1)
	{
		const uint32_t Index = MeshData.VertexData.size();

		const float Y = 0.5f * InHeight;
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			const float RCos = cosf(i * BetaValue);
			const float RSin = sinf(i * BetaValue);

			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * RCos,//x
					Y,//y
					InTopRadius * RSin), //z
				XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.f)));

			FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

			//չUV
			MyVertex.TexCoord.x = RCos;
			MyVertex.TexCoord.y = RSin;
		}

		//����е�
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White), XMFLOAT3(0.f,1.f,0.f)));

		//����indexģ��
		const float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i + 1);
			MeshData.IndexData.push_back(Index + i);
		}
	}

	//�����ײ�
	if (1)
	{
		const uint32_t Index = MeshData.VertexData.size();

		const float Y = -0.5f * InHeight;
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			const float RCos = cosf(i * BetaValue);
			const float RSin = sinf(i * BetaValue);

			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InBottomRadius * RCos,//x
					Y,//y
					InBottomRadius * RSin), //z
				XMFLOAT4(Colors::White),XMFLOAT3(0.f, -1.f, 0.f)));

			FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

			//չUV
			MyVertex.TexCoord.x = RCos;
			MyVertex.TexCoord.y = RSin;
		}

		//����е�
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));

		//����indexģ��
		const float CenterPoint = MeshData.VertexData.size() - 1;
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			MeshData.IndexData.push_back(CenterPoint);
			MeshData.IndexData.push_back(Index + i);
			MeshData.IndexData.push_back(Index + i + 1);
		}
	}
}

void CCylinderMeshComponent::BuildKey(size_t& OutHashKey, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	const std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 4;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InBottomRadius);
	OutHashKey += FloatHash(InTopRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
