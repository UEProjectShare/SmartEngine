#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Mesh/Core/MeshType.h"

class CMeshComponent;
enum ERenderingMeshType;

struct FRenderingDataSection : public FMeshSection
{
	FRenderingDataSection();

	UINT VertexOffsetPosition;//�ֲ�ƫ��
	
	UINT IndexOffsetPosition;//Indexƫ��
	
	int MeshObjectIndex;//Index
};

//�ṩ��Ⱦ���ݵĽӿ�
struct FRenderingData 
	: public IDirectXDeviceInterface_Struct
	, public std::enable_shared_from_this<FRenderingData>
{
public:
	FRenderingData();
	
	std::vector<FRenderingDataSection> Sections;
	
	UINT IndexTotalSize;
	
	UINT VertexTotalxSize;

	UINT VertexOffsetPosition;//������ƫ��
	
	UINT VertexTypeSize;//�������͵Ĵ�С
	
	UINT IndexTypeSize;//Index���ʹ�С
	
	BoundingBox Bounds;//AABB

	size_t MeshHash;
	
	int GeometryKey;
	
	ERenderingMeshType GetMeshType() const;
	
	FVertexMeshData* GetMeshRenderingData();
	
	FSkinnedVertexMeshData* GetSkinnedMeshRenderingData();
	
	UINT GetVertexSizeInBytes()const { return VertexTotalxSize * VertexTypeSize; }
	
	UINT GetIndexSizeInBytes()const { return IndexTotalSize * IndexTypeSize; }
	
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;
	
	XMFLOAT4X4 TextureTransform;

	CMeshComponent* Mesh;//key

	//�п�������Ƥ�п����Ǿ�̬����
	void *MeshRenderingData;
};