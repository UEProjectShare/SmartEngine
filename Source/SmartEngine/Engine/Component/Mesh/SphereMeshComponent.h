#pragma once
#include "Core/ShellMeshComponent.h"
#include "SphereMeshComponent.CodeReflection.h"

class CSphereMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CSphereMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse = false);

	void BuildKey(size_t& OutHashKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse);
};
