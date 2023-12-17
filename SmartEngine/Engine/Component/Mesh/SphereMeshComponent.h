#pragma once
#include "ShellMeshComponent.h"

class CSphereMeshComponent : public CShellMeshComponent
{
public:
	CSphereMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
