#pragma once
#include "Core/ShellMeshComponent.h"
#include "ConeMeshComponent.CodeReflection.h"

class CConeMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CConeMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	void BuildKey(size_t& OutHashKey,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};
