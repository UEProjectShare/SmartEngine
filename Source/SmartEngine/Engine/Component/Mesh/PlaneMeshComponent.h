#pragma once
#include "Core/ShellMeshComponent.h"
#include "PlaneMeshComponent.CodeReflection.h"

class CPlaneMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CPlaneMeshComponent();
	
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};
