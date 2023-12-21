#include "PyramidMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPyramidMeshComponent::CPyramidMeshComponent()
{

}

void CPyramidMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	EPyramidNumberSides PyramidNumberSides,
	uint32_t InHeightSubdivide, uint32_t InSize)
{
	const float InRadius = InSize;
	const float InHeight = InSize * 2.f;
	const uint32_t InAxialSubdivision = static_cast<uint32_t>(PyramidNumberSides);

	//�뾶���
	const float RadiusInterval = -InRadius / static_cast<float>(InHeightSubdivide);
	//�߶ȼ��
	const float HeightInterval = InHeight / static_cast<float>(InHeightSubdivide);

	const float BetaValue = XM_2PI / static_cast<float>(InAxialSubdivision);

	//��������
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, 0.5f * InHeight, 0.f),
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, 1.f, 0.f)));

	for (uint32_t i = 0; i <= InHeightSubdivide; ++i)
	{
		const float Y = 0.5f * InHeight - HeightInterval * i;
		const float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float c = cosf(j * BetaValue);
			const float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White)));

			FVertex& Vertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			Vertex.UTangent = XMFLOAT3(-s, Y, c);

			float dr = InRadius;
			XMFLOAT3 bitangent(dr * c, -InHeight, dr * s);

			const XMVECTOR T = XMLoadFloat3(&Vertex.UTangent);
			const XMVECTOR B = XMLoadFloat3(&bitangent);
			const XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&Vertex.Normal, N);
		}
	}

	//����е�
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, -0.5f * InHeight, 0.f),
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, -1.f, 0.f)));

	//����indexģ��
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	float BaseIndex = 1;
	const float VertexCircleNum = InAxialSubdivision + 1;
	//������Χ
	for (uint32_t i = 0; i <= InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//���ǻ��Ƶ����ı���
			//������1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//������2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	//����ϼ���
	{
		const int i = InHeightSubdivide;

		const float Y = 0.5f * InHeight - HeightInterval * i;
		const float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			const float c = cosf(j * BetaValue);
			const float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));
		}
	}

	//�����ϼ�
	const uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;
	BaseIndex = SouthBaseIndex - VertexCircleNum;
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)
	{
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + Index);
		MeshData.IndexData.push_back(BaseIndex + Index + 1);
	}
}
