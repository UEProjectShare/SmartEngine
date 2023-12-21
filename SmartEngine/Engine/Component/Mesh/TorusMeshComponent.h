#pragma once
#include "Core/ShellMeshComponent.h"

class CTorusMeshComponent : public CShellMeshComponent
{
public:
	CTorusMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
