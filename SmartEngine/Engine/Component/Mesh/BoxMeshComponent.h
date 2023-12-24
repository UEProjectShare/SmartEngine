#pragma once
#include "Core/ShellMeshComponent.h"

class CBoxMeshComponent : public CShellMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);

	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, float InDepth);
};
