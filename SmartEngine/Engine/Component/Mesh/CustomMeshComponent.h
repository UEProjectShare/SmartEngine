#pragma once
#include "Core/MeshComponent.h"

//模型读取类型
enum EMeshLoadAssetType
{
	MESH_OBJ,
	MESH_FBX,
};

class CCustomMeshComponent : public CMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, const string& InPath);

	static bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);
	
	static bool LoadFBXFromBuff(const string& InPath, FMeshRenderingData& MeshData);

	void BuildKey(size_t& OutHashKey, const std::string& InPath);
};
