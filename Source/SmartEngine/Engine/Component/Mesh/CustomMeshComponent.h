#pragma once
#include "Core/MeshComponent.h"
#include "CustomMeshComponent.CodeReflection.h"

//模型读取类型
enum EMeshLoadAssetType
{
	MESH_OBJ,
	MESH_FBX,
};

struct FIEParam;

class CCustomMeshComponent : public CMeshComponent
{
	CODEREFLECTION()
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, const string& InPath, const FIEParam& InParam);

	bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);
	
	bool LoadFBXFromBuff(const string& InPath, FMeshRenderingData& MeshData, const FIEParam& InParam);

	void BuildKey(size_t& OutHashKey, const std::string& InPath, const FIEParam& InParam);
};
