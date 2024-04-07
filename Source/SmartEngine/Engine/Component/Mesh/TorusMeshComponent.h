#pragma once
#include "Core/ShellMeshComponent.h"
#include "TorusMeshComponent.CodeReflection.h"

class CTorusMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CTorusMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	void BuildKey(size_t& OutHashKey, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
