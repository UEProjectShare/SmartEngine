#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"

class GMesh;
//�ṩ��Ⱦ���ݵĽӿ�
struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();

	UINT IndexSize;//Index������
	
	UINT VertexSize;//Vertex������

	UINT IndexOffsetPosition;
	
	UINT VertexOffsetPosition;

	UINT VertexTypeSize;
	
	UINT IndexTypeSize;
	
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; }
	
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; }
	
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;

	GMesh* Mesh;//key
};