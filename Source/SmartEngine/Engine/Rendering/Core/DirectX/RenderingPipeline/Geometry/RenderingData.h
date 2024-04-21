#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Mesh/Core/MeshType.h"

class CMeshComponent;
enum ERenderingMeshType;

struct FRenderingDataSection : public FMeshSection
{
	FRenderingDataSection();

	UINT VertexOffsetPosition;//局部偏移
	
	UINT IndexOffsetPosition;//Index偏移
	
	int MeshObjectIndex;//Index
};

//提供渲染内容的接口
struct FRenderingData 
	: public IDirectXDeviceInterface_Struct
	, public std::enable_shared_from_this<FRenderingData>
{
public:
	FRenderingData();
	
	std::vector<FRenderingDataSection> Sections;
	
	UINT IndexTotalSize;
	
	UINT VertexTotalSize;

	UINT VertexOffsetPosition;//顶点总偏移
	
	UINT VertexTypeSize;//顶点类型的大小
	
	UINT IndexTypeSize;//Index类型大小
	
	BoundingBox Bounds;//AABB

	size_t MeshHash;
	
	int GeometryKey;
	
	ERenderingMeshType GetMeshType() const;
	
	FVertexMeshData* GetMeshRenderingData() const;
	
	FSkinnedVertexMeshData* GetSkinnedMeshRenderingData() const;
	
	UINT GetVertexSizeInBytes()const { return VertexTotalSize * VertexTypeSize; }
	
	UINT GetIndexSizeInBytes()const { return IndexTotalSize * IndexTypeSize; }
	
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;
	
	XMFLOAT4X4 TextureTransform;

	CMeshComponent* Mesh;//key

	//有可能是蒙皮有可能是静态网格
	void *MeshRenderingData;
};