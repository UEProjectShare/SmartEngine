#pragma once
#include "Core/ShellMeshComponent.h"
#include "BoxMeshComponent.CodeReflection.h"

class CBoxMeshComponent : public CShellMeshComponent
{
	CODEREFLECTION()
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);

	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, float InDepth);
};
