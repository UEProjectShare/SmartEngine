#pragma once
#include "Core/MeshComponent.h"
#include "SkinnedMeshComponent.CodeReflection.h"

//模型读取类型
class CSkinnedMeshComponent : public CMeshComponent
{
	CODEREFLECTION()
public:
	CSkinnedMeshComponent();

	void CreateMesh(FSkinnedMeshRenderingData& MeshData, const string& InPath);

	ERenderingMeshType GetMeshType() const override { return ERenderingMeshType::SKINNED_MESH_TYPE; }
	
	void BuildKey(size_t& OutHashKey, const std::string& InPath);
};