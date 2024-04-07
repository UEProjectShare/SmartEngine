#pragma once
#include "Core/ShellMeshComponent.h"
#include "CylinderMeshComponent.CodeReflection.h"

class CCylinderMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CCylinderMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	void BuildKey(size_t& OutHashKey,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};
