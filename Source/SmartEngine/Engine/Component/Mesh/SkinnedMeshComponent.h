#pragma once
#include "Core/MeshComponent.h"
#include "SkinnedMeshComponent.CodeReflection.h"

//模型读取类型
class CSkinnedMeshComponent :public CMeshComponent
{
	CODEREFLECTION()
public:
	CSkinnedMeshComponent();

	void CreateMesh(FSkinnedMeshRenderingData& MeshData, const string& InPath);

	virtual ERenderingMeshType GetMeshType() const { return ERenderingMeshType::SKINNED_MESH_TYPE; }

public:
	void BuildKey(size_t& OutHashKey, const std::string& InPath);
};